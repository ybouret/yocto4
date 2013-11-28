#include "yocto/math/ztype.hpp"
#include "yocto/math/sig/extend.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/math/types.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/math/kernel/lu.hpp"
#include "yocto/code/ipower.hpp"

#include <cerrno>

#include "yocto/ios/ocstream.hpp"
#include "yocto/sort/quick.hpp"

namespace yocto
{
    namespace math
    {
        
        
        static inline int compare_v(const v2d<real_t> &lhs, const v2d<real_t> &rhs ) throw()
        {
            return __compare(lhs.x, rhs.x);
        }
        
        template <> extend<real_t>:: ~extend() throw() {}
        
        template <> extend<real_t>:: extend(extend_mode lo,
                                            extend_mode up) throw() :
        lower(lo),
        upper(up)
        {}
        
        template <> extend<real_t>:: extend( extend_mode both ) throw() :
        lower( both ),
        upper( both )
        {
        }
        
        
        
#if 0
        template <>
        real_t extend<real_t>:: operator()(real_t               x,
                                           const array<real_t> &X,
                                           const array<real_t> &Y) const throw()
        {
            assert(X.size()==Y.size());
            assert(X.size()>=2);
            const extend &self = *this;
            const size_t N = X.size();
            const real_t X1 = X[1];
            const real_t XN = X[N];
            if(x <= X1 )
            {
                switch(lower)
                {
                    case extend_constant:
                        return Y[1];
                        
                        
                    case extend_cyclic:
                        return self(x+(XN-X1),X,Y);
                }
            }
            else
            {
                if(x>=XN)
                {
                    switch(upper)
                    {
                        case extend_constant:
                            return Y[N];
                            
                        case extend_cyclic:
                            return self( x-(XN-X1),X,Y);
                    }
                }
                else
                {
                    size_t klo = 1, khi = N;
					while( (khi-klo)> 1 )
					{
						const size_t k = (klo+khi)>>1;
						if( X[k] > x )
							khi = k;
						else
							klo = k;
					}
                    return Y[klo] + (x-X[klo]) * (Y[khi] - Y[klo]) / (X[khi] - X[klo]);
                    
                }
            }
            return 0;
        }
#endif
        
        template <>
        real_t extend<real_t>:: get_x( ptrdiff_t i, const array<real_t> &X, const ptrdiff_t N, const real_t L ) const throw()
        {
            if(i<1)
            {
                switch(lower)
                {
                        
                    case extend_cyclic:
                    {
                        real_t dx = 0;
                        while(i<1)
                        {
                            i  += N;
                            dx += L;
                        }
                        assert(i<=N);
                        return X[i] - dx;
                    }
                        
                    default:
                        return X[1]+(X[1] - get_x(2-i, X, N, L));
                }
            }
            else
            {
                if(i>N)
                {
                    switch(upper)
                    {
                        case extend_cyclic:
                        {
                            real_t dx =0;
                            while(i>N)
                            {
                                i -=N;
                                dx+=L;
                            }
                            assert(i>=1);
                            return X[i] + dx;
                        }
                            
                        default:
                            return X[N] + (X[N] - get_x(N+N-i,X,N,L));
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
                    case extend_constant:
                        return Y[1];
                        
                    case extend_cyclic:
                        return get_y(i+N,Y,N);
                        
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
                        case extend_constant:
                            return Y[N];
                            
                        case extend_cyclic:
                            return get_y(i-N,Y,N);
                            
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
            const real_t L = X[N] - X[1];
            v2d<real_t> ans;
            ans.x = get_x(i, X, N, L);
            ans.y = get_y(i, Y, N );
            
            return ans;
        }
        
        template <>
        void extend<real_t>:: operator()(extend_value         kind,
                                         array<real_t>       &Z,
                                         const array<real_t> &X,
                                         const array<real_t> &Y,
                                         real_t               dt_prev,
                                         real_t               dt_next,
                                         const size_t         degree
                                         ) const
        {
            assert(X.size()==Y.size());
            assert(Z.size()==Y.size());
            
            //-- sort out sizes
            switch( X.size() )
            {
                case 0:
                    return;
                    
                case 1:
                    switch(kind)
                {
                    case extend_eval: Z[1] = Y[1]; break;
                    case extend_diff: Z[1] = 0;    break;
                }
                    return;
                    
                default:
                    break;
            }
            
            size_t nmin = 0;
            
            switch(kind)
            {
                case extend_eval: nmin=0; break;
                case extend_diff: nmin=1; break;
            }
            
            const ptrdiff_t N = ptrdiff_t(X.size());
            if(N<=0)
                throw libc::exception( ERANGE, "integer overflow in extend()");
            
            const size_t ncoef = degree+1;
            const real_t L     = X[N] - X[1];
            dt_prev = Fabs(dt_prev);
            dt_next = Fabs(dt_next);
            vector< v2d<real_t> > v;
            for(ptrdiff_t i=1;i<=N;++i)
            {
                v.free();
                const real_t xi = X[i];
                v2d<real_t> tmp(0,Y[i]);
                v.push_back(tmp);
                
                size_t nl = 0;
                for(ptrdiff_t j=i-1;;--j)
                {
                    const real_t xj = get_x(j, X, N, L);
                    const real_t dx = xj-xi;
                    if( nl>=nmin && Fabs(dx) > dt_prev )
                        break;
                    
                    tmp.x = dx;
                    tmp.y = get_y(j, Y, N);
                    v.push_back(tmp);
                    ++nl;
                }
                
                size_t nr = 0;
                for(ptrdiff_t j=i+1;;++j)
                {
                    const real_t xj = get_x(j, X, N, L);
                    const real_t dx = xj-xi;
                    if( nr>=nmin && Fabs(dx) > dt_next )
                        break;
                    tmp.x = dx;
                    tmp.y = get_y(j, Y, N);
                    v.push_back(tmp);
                    ++nr;
                }
                const size_t W = v.size(); assert(W>=1+2*nmin);
                
                //______________________________________________________________
                //
                // number of coefficients
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
                lu<real_t> LU(m);
                if( !LU.build(mu) )
                    throw exception("invalid data @X[%u]=%g", unsigned(i), double(X[i]));
                LU.solve(mu,a);
                switch(kind)
                {
                    case extend_eval:                      Z[i] = a[1]; break;
                    case extend_diff: assert(a.size()>=2); Z[i] = a[2]; break;
                }
            }
            
            
        }
        
        
        
    }
}
