#include "yocto/math/ztype.hpp"
#include "yocto/math/sig/extend.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/math/types.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/math/kernel/crout.hpp"
#include "yocto/code/ipower.hpp"

#include <cerrno>

#include "yocto/ios/ocstream.hpp"
#include "yocto/sort/quick.hpp"

namespace yocto
{
    namespace math
    {
        
        
        ////////////////////////////////////////////////////////////////////////
        //
        // extend
        //
        ////////////////////////////////////////////////////////////////////////
        template <>
        extend<real_t>:: ~extend() throw() {}
        
        template <>
        extend<real_t>:: extend(extend_mode lo,
                                extend_mode up) :
        lower(lo),
        upper(up)
        {
            const bool valid = (upper==extend_cyclic&&lower==extend_cyclic) || (upper!=extend_cyclic&&lower!=extend_cyclic);
            if(!valid)
                throw exception("extend: both sides are cyclic or not");
            
        }
        
        template <>
        extend<real_t>:: extend( extend_mode both ) throw() :
        lower( both ),
        upper( both )
        {
            
        }
        
        
        
        
        template <>
        real_t extend<real_t>:: get_x(ptrdiff_t i, const array<real_t> &X, const ptrdiff_t N) const throw()
        {
            if(i<1)
            {
                switch(lower)
                {
                        
                    case extend_cyclic:
                        return get_x(i+N-1,X,N) - (X[N]-X[1]);
                        
                    default:
                        return X[1]+(X[1] - get_x(2-i, X, N));
                }
            }
            else
            {
                if(i>N)
                {
                    switch(upper)
                    {
                        case extend_cyclic:
                            return get_x(i-N+1,X,N)+(X[N]-X[1]);
                            
                        default:
                            return X[N] + (X[N] - get_x(N+N-i,X,N));
                    }
                }
                else
                    return X[i];
            }
            
        }
        
        template <>
        real_t extend<real_t>:: get_y( ptrdiff_t i, const array<real_t> &Y, const ptrdiff_t N ) const throw()
        {
            if(i<1)
            {
                switch(lower)
                {
                    case extend_zero:
                        return 0;
                        
                    case extend_constant:
                        return Y[1];
                        
                    case extend_cyclic:
                        return get_y(i+N-1,Y,N);
                        
                    case extend_odd:
                        return Y[1] - (get_y(2-i,Y,N)-Y[1]);
                        
                    case extend_even:
                        return get_y(2-i,Y,N);
                }
            }
            else
            {
                if(i>N)
                {
                    switch(upper)
                    {
                        case extend_zero:
                            return 0;
                            
                        case extend_constant:
                            return Y[N];
                            
                        case extend_cyclic:
                            return get_y(i-N+1,Y,N);
                            
                        case extend_odd:
                            return Y[N] - (get_y(N+N-i,Y,N)-Y[N]);
                            
                        case extend_even:
                            return get_y(N+N-i,Y,N);
                            
                    }
                }
                else
                {
                    return Y[i];
                }
            }
            return 0; // never get here
        }
        
        
        template <>
        v2d<real_t> extend<real_t>:: at( ptrdiff_t i, const array<real_t> &X, const array<real_t> &Y) const
        {
            
            assert(X.size()>0);
            assert(Y.size()>0);
            assert(X.size()==Y.size());
            const ptrdiff_t N = ptrdiff_t(X.size());
            
            if(N<=0)
                throw libc::exception( ERANGE, "integer overflow in extend()");
            v2d<real_t> ans;
            return v2d<real_t>( get_x(i, X, N ), get_y(i, Y, N ) );
        }
        
        
        namespace
        {
            static inline int __compare_x( const v2d<real_t> &lhs, const v2d<real_t> &rhs ) throw()
            {
                return (lhs.x < rhs.x) ? -1 : (rhs.x<lhs.x? 1 : 0);
            }
            
        }
        template <>
        real_t extend<real_t>:: operator()(array<real_t>       &Z,
                                           const array<real_t> &X,
                                           const array<real_t> &Y,
                                           real_t               dt_prev,
                                           real_t               dt_next,
                                           const size_t         degree,
                                           array<real_t>       *dZdX
                                           ) const
        {
            assert(X.size()==Y.size());
            assert(Z.size()==Y.size());
            //__________________________________________________________________
            //
            // setup
            //__________________________________________________________________
            const bool drvs = dZdX != 0;
            size_t nmin = 0;
            if(drvs)
            {
                assert(dZdX->size() == X.size());
                nmin = 1; // take at least one point on each side
            }
            
            //__________________________________________________________________
            //
            // sort out sizes
            //__________________________________________________________________
            switch( X.size() )
            {
                case 0:
                    return 0;
                    
                case 1:
                    Z[1] = Y[1];
                    if(drvs) (*dZdX)[1] = 0;
                    return 0;
                    
                    
                default:
                    break;
            }
            
            //__________________________________________________________________
            //
            // Generic Case
            //__________________________________________________________________
            const ptrdiff_t N = ptrdiff_t(X.size());
            if(N<=0)
                throw libc::exception( ERANGE, "integer overflow in extend()");
            real_t       rms   = 0;             // compute RMS smooth vs. signal
            const size_t ncoef = degree+1;      // desired #polynomial coefficients
            dt_prev = Fabs(dt_prev);
            dt_next = Fabs(dt_next);
            vector< v2d<real_t> > v(64,as_capacity); // local approximation
            
            for(ptrdiff_t i=1;i<=N;++i)
            {
                v.free();
                const real_t xi = X[i];  // central point
                v2d<real_t> tmp(0,Y[i]); // central value
                v.push_back(tmp);
                
                if(extend_cyclic==lower)
                {
                    assert(extend_cyclic==upper);
                    //______________________________________________________________
                    //
                    // Everybody looks to the left...
                    //______________________________________________________________
                    size_t nl = 0;
                    for(ptrdiff_t j=i-1;;--j)
                    {
                        const real_t xj = get_x(j, X, N);
                        const real_t dx = xj-xi;
                        if( nl>=nmin && Fabs(dx) > dt_prev )
                            break;
                        
                        tmp.x = dx;
                        tmp.y = get_y(j, Y, N);
                        v.push_back(tmp);
                        ++nl;
                    }
                    
                    //______________________________________________________________
                    //
                    // Everybody looks to the right...
                    //______________________________________________________________
                    size_t nr = 0;
                    for(ptrdiff_t j=i+1;;++j)
                    {
                        const real_t xj = get_x(j, X, N);
                        const real_t dx = xj-xi;
                        if( nr>=nmin && Fabs(dx) > dt_next )
                            break;
                        tmp.x = dx;
                        tmp.y = get_y(j, Y, N);
                        v.push_back(tmp);
                        ++nr;
                    }
                }
                else
                {
                    // calibrate w.r.t to the closest side
                    throw exception("not implemented");
                }
                
                //______________________________________________________________
                //
                // We have a window size
                //______________________________________________________________
                const size_t W = v.size(); assert(W>=1+2*nmin);
                
                //______________________________________________________________
                //
                // and a number of coefficients
                //______________________________________________________________
                const size_t m = min_of<size_t>(ncoef,W);
                
                matrix<real_t>  mu(m,m);
                vector<real_t>  a(m,0.0);
                
                //______________________________________________________________
                //
                // rhs
                //______________________________________________________________
                for(size_t r=1;r<=m;++r)
                {
                    const size_t p = r-1;
                    real_t       s = 0;
                    for(size_t j=1;j<=W;++j)
                    {
                        const v2d<real_t> &q = v[j];
                        s += q.y * ipower(q.x,p);
                    }
                    a[r] = s;
                }
                
                //______________________________________________________________
                //
                // momenta
                //______________________________________________________________
                for(size_t r=1;r<=m;++r)
                {
                    for(size_t c=r;c<=m;++c)
                    {
                        const size_t p=(c+r-2);
                        real_t       s=0;
                        for(size_t j=1;j<=W;++j)
                        {
                            s += ipower(v[j].x,p);
                        }
                        mu[r][c] = mu[c][r] = s;
                    }
                }
                
                
                
                //______________________________________________________________
                //
                // solve for coefficients
                //______________________________________________________________
                crout<real_t> LU(m);
                if( !LU.build(mu) )
                    throw exception("singular extended data @X[%u]=%g", unsigned(i), double(X[i]));
                crout<real_t>::solve(mu,a);
                
                //______________________________________________________________
                //
                // get the approximated central value
                //______________________________________________________________
                Z[i] = a[1];
                const real_t dz = Z[i] - Y[i];
                rms += dz*dz;
                if(drvs)
                {
                    //__________________________________________________________
                    //
                    // get the approximated derivative
                    //__________________________________________________________
                    (*dZdX)[i] = a[2];
                }
                
#if 0
                {
                    ios::ocstream fp("window.dat",false);
                    quicksort(v,__compare_x);
                    for(size_t j=1;j<=W;++j)
                    {
                        const v2d<real_t> &q = v[j];
                        real_t ans = a[1];
                        for(size_t k=2;k<=m;++k)
                        {
                            ans += a[k] * ipower(q.x,k-1);
                        }
                        real_t tg = 0;
                        if(m>=2)
                        {
                            tg = a[1] + q.x * a[2];
                        }
                        fp("%g %g %g %g\n",q.x+xi,q.y,ans,tg);
                    }
                    exit(0);
                }
#endif
                
            }
            return Sqrt( rms/N );
        }
        
        template <>
        void extend<real_t>:: build_rms(sequence<real_t>    &dx,
                                        sequence<real_t>    &rms,
                                        const array<real_t> &X,
                                        const array<real_t> &Y,
                                        const size_t         degree) const
        {
            const extend<real_t> &self = *this;
            dx.free();
            rms.free();
            const size_t N = X.size();
            if(N<2)
            {
                dx.push_back(0);
                rms.push_back(0);
            }
            else
            {
                vector<real_t> Z(N,0);
                const real_t   width = X[N] - X[1];
                const real_t   xstep = width/(N-1);
                const size_t   M     = N/2;
                for(size_t i=0; i <= M; ++i)
                {
                    const real_t dx_value  = i * xstep;
                    const real_t rms_value = self(Z,X,Y,dx_value,degree,0);
                    dx.push_back(dx_value);
                    rms.push_back(rms_value);
                }
            }
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // extend
        //
        ////////////////////////////////////////////////////////////////////////
        template <> extend2<real_t>:: ~extend2()  throw() {}
        
        template <>
        extend2<real_t>:: extend2(extend_mode lo, extend_mode up) throw() :
        func(lo,up),
        diff(extend_mode_for_derivative(lo),extend_mode_for_derivative(up))
        {
            
        }
        
        template <>
        extend2<real_t>:: extend2(extend_mode both) throw() :
        func(both),
        diff(extend_mode_for_derivative(both))
        {
        }
        
        template <>
        real_t extend2<real_t>::operator()(array<real_t>       &Z,
                                           const array<real_t> &X,
                                           const array<real_t> &Y,
                                           const real_t         dt_prev,
                                           const real_t         dt_next,
                                           const size_t         degree,
                                           array<real_t>       &dZdX) const
        {
            vector<real_t> approx( X.size(), REAL(0.0) );
            const real_t ans = func(Z,X,Y,dt_prev,dt_next,degree,&approx);
            diff( dZdX, X, approx,dt_prev,dt_next, (degree>=1) ? degree-1 : 0, 0);
            //diff( dZdX, X, approx,dt_prev,dt_next, degree, 0);
            return ans;
        }
        
        
        
        
    }
}
