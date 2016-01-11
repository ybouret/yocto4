#include "yocto/math/ztype.hpp"
#include "yocto/math/types.hpp"
#include "yocto/math/sig/smoother.hpp"
#include "yocto/exception.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/code/ipower.hpp"
#include "yocto/math/core/lu.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/ios/ocstream.hpp"


namespace yocto
{
    namespace math
    {
        template <>
        smoother<real_t>:: ~smoother() throw()
        {
        }

        template <>
        smoother<real_t>:: smoother() throw() :
        lower_range(0),
        upper_range(0),
        degree(0),
        v()
        {

        }

        namespace
        {
            static inline int __compare_x( const v2d<real_t> &lhs, const v2d<real_t> &rhs ) throw()
            {
                return (lhs.x < rhs.x) ? -1 : (rhs.x<lhs.x? 1 : 0);
            }
        }


        template <>
        real_t smoother<real_t>:: operator()(size_t                  i,
                                             const array<real_t>    &X,
                                             const array<real_t>    &Y,
                                             const extender<real_t> &E,
                                             real_t                 *dYdX)
        {
            assert(X.size()==Y.size());
            assert(i>0);
            assert(i<=X.size());
            assert(upper_range>=0);
            assert(lower_range>=0);

            //__________________________________________________________________
            //
            // Initialize
            //__________________________________________________________________
            const ptrdiff_t N = ptrdiff_t(X.size());
            switch(N)
            {
                case 0:
                    if(dYdX) *dYdX = 0;
                    return 0;

                case 1:
                    if(dYdX) *dYdX = 0;
                    return Y[1];

                default:
                    break;
            }

            // prepare center point
            v.free();
            vtx_t tmp(0,Y[i]);
            v.push_back(tmp);
            const real_t xi = X[i];



            {
                const size_t nmin = dYdX ? 1 : 0;
                size_t       nl   = 0;


                // got to lower bound
                for(ptrdiff_t j=i-1;;--j,++nl)
                {
                    const real_t dx = E.get_x(j, X, N)-xi;
                    if(nl>nmin && Fabs(dx)>lower_range)
                    {
                        break;
                    }
                    const real_t y  = E.get_y(j, Y, N);
                    const vtx_t  tmp(dx,y);
                    v.push_front(tmp);
                }
                assert(1+nl==v.size());

                size_t       nr   = 0;
                for(ptrdiff_t j=i+1;;++j,++nr)
                {
                    const real_t dx = E.get_x(j, X, N) - xi;
                    if(nr>nmin&&Fabs(dx)>upper_range)
                    {
                        break;
                    }
                    const real_t y  = E.get_y(j, Y, N);
                    const vtx_t  tmp(dx,y);
                    v.push_back(tmp);
                }
                assert(1+nr+nl==v.size());
                assert(v.size()>2*nmin);
            }

            //__________________________________________________________________
            //
            // window size
            //__________________________________________________________________
            const size_t W = v.size();

            //__________________________________________________________________
            //
            // polynomial #coefficients
            //__________________________________________________________________
            const size_t   m = min_of<size_t>(degree+1,W);


            //__________________________________________________________________
            //
            // prepare the rhs
            //__________________________________________________________________
            vector<real_t>  a(m,REAL(0.0));

            for(size_t r=1;r<=m;++r)
            {
                const size_t p=r-1;
                real_t       s=0;
                list<vtx_t>::iterator i = v.begin();
                for(size_t j=W;j>0;--j,++i)
                {
                    const vtx_t &q = *i;
                    s += q.y * ipower(q.x,p);
                }
                a[r] = s;
            }

            //__________________________________________________________________
            //
            // prepare the momenta
            //__________________________________________________________________
            matrix<real_t> mu(m);
            for(size_t r=1;r<=m;++r)
            {
                for(size_t c=r;c<=m;++c)
                {
                    const size_t p=(r+c-2);
                    real_t       s=0;
                    list<vtx_t>::iterator i = v.begin();
                    for(size_t j=W;j>0;--j,++i)
                    {
                        const vtx_t &q = *i;
                        s += ipower(q.x,p);
                    }
                    mu[r][c] = mu[c][r] = s;
                }
            }

            //__________________________________________________________________
            //
            // solve the polynomial coefficients
            //__________________________________________________________________
            if( !LU<z_type>::build(mu) )
                throw exception("singular smoothing window around X[%d]=%g", int(i), xi);
            LU<real_t>::solve(mu,a);

            //__________________________________________________________________
            //
            // store the derivative if necessary
            //__________________________________________________________________
            if( dYdX )
            {
                assert(m>=2);
                *dYdX = a[2];
            }

#if 0
            if(i==1||i==N||i==N/2)
            {

                ios::ocstream fp( vformat("window%d.dat",int(i)), false);
                //v.sort( __compare_x );
                for(list<vtx_t>::iterator i=v.begin();i!=v.end();++i)
                {
                    const vtx_t &q = *i;
                    real_t f = a[1];
                    for(size_t k=2;k<=m;++k) f += a[k] * ipower(q.x,k-1);
                    fp("%g %g %g\n", q.x+xi, q.y,f);
                }
            }
#endif
            //__________________________________________________________________
            //
            // return the smoothed value
            //__________________________________________________________________
            return a[1];
        }


        template <>
        void smoother<real_t>:: operator()(array<real_t>          &Z,
                                           const array<real_t>    &X,
                                           const array<real_t>    &Y,
                                           const extender<real_t> &E,
                                           array<real_t>          *dZdX)
        {
            assert(Z.size()==X.size());
            assert(Z.size()==Y.size());
            const size_t N = Z.size();
            smoother<real_t> &self = *this;

            if(dZdX)
            {
                assert(dZdX->size()==Z.size());
                array<real_t> &diff = *dZdX;
                for(size_t i=1;i<=N;++i)
                    Z[i] = self(i,X,Y,E, &diff[i]);
            }
            else
            {
                for(size_t i=1;i<=N;++i)
                    Z[i] = self(i,X,Y,E, 0);
            }

        }
    }
}

#include "yocto/auto-clean.hpp"

namespace yocto
{
    namespace math
    {

        namespace
        {
            static inline
            extension_type __drvs_ext( extension_type t )
            {
                switch(t)
                {
                    case extend_cyclic:
                        return extend_cyclic;

                    case extend_odd:
                        return extend_even;

                    case extend_even:
                        return extend_odd;

                    case extend_constant:
                    case extend_zero:
                        break;
                }
                return extend_zero;
            }
        }

        template <>
        void smoother<real_t>:: operator()(array<real_t>          &Z,
                                           const array<real_t>    &X,
                                           const array<real_t>    &Y,
                                           const extender<real_t> &E,
                                           array<real_t>          &dZdX)
        {
            smoother<real_t> &self = *this;
            const size_t N = Z.size();
            vector<real_t> diff(N,REAL(0.0));
            self(Z,X,Y,E,&diff);
            const extender<real_t> D( __drvs_ext(E.lower), __drvs_ext(E.upper) );
            const size_t backup = degree;
            if(degree>0) --degree;

            YOCTO_FAILSAFE(self(dZdX,X,diff,D,0),degree=backup);
            // try { self(dZdX,X,diff,D,0); degree=backup; }catch(...){ degree=backup; throw; }
        }
        
        
    }
}
