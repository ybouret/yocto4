#ifndef YOCTO_MATH_SIG_SMOOTH
#define YOCTO_MATH_SIG_SMOOTH 1

#include "yocto/math/sig/expand.hpp"
#include "yocto/math/point2d.hpp"
#include "yocto/sequence/list.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/math/core/lu.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/code/ipower.hpp"

#include "yocto/exceptions.hpp"
#include <cerrno>

#include "yocto/ios/ocstream.hpp"

namespace yocto
{
    namespace math
    {

        template <typename T>
        class smooth
        {
        public:
            typedef point2d<T>    point_t;
            typedef list<point_t> points_t;

            inline explicit smooth() throw() :
            points(),
            lower_range(0),
            upper_range(0),
            degree(0),
            mu(),
            coeff()
            {}

            inline virtual ~smooth() throw() {}

            points_t      points;      //!< local data points
            T             lower_range; //!< how far before current value
            T             upper_range; //!< how far beyond current value
            size_t        degree;      //!< desired degreee


            //! compute the approximation of Y[i]@X[i]
            inline T compute(const expand<T> &xp,
                             const unit_t     i,
                             const array<T>  &X,
                             const array<T>  &Y,
                             const unit_t     N,
                             T               *dYdX=0)
            {
                assert(N>0);
                assert(X.size()==Y.size());
                assert(unit_t(X.size())==N);

                //______________________________________________________________
                //
                // prepare with central point
                //______________________________________________________________
                const T Xc = xp.get_x(i,X,N);
                const T Yc = xp.get_y(i,Y,N);
                points.free();
                push_back(0,Yc);

                //______________________________________________________________
                //
                // add next points
                //______________________________________________________________
                {
                    const T Xup = Xc + max_of<T>(0,upper_range);
                    unit_t  iup = i;
                    size_t  cnt = 0;
                    while(true)
                    {
                        const T Xi=xp.get_x(++iup,X,N);
                        if(Xi>Xup&&cnt>0) break;
                        ++cnt;
                        push_back(Xi-Xc,xp.get_y(iup,Y,N));
                    }
                }

                //______________________________________________________________
                //
                // add prev points
                //______________________________________________________________
                {
                    const T Xdn = Xc - max_of<T>(0,lower_range);
                    unit_t  idn = i;
                    size_t  cnt = 0;
                    while(true)
                    {
                        const T Xi=xp.get_x(--idn,X,N);
                        if(Xi<Xdn&&cnt>0) break;
                        ++cnt;
                        push_front(Xi-Xc,xp.get_y(idn,Y,N));
                    }
                }

                //______________________________________________________________
                //
                // use local fit
                //______________________________________________________________
                polynomial();
                if(dYdX)
                {
                    *dYdX = coeff[2];
                }
                return coeff[1];
            }


            //______________________________________________________________
            //
            // one pass computation of smoothed data,
            // with derivative approximation
            //______________________________________________________________
            inline void operator()(const expand<T> &xp,
                                   array<T>        &Z,
                                   const array<T>  &X,
                                   const array<T>  &Y,
                                   array<T>        *dYdX)
            {
                assert(X.size()==Y.size());
                assert(Y.size()==Z.size());
                assert(X.size()>0);


                const unit_t N(X.size());
                if(dYdX)
                {
                    assert(dYdX->size() == X.size());
                    array<T> &dy = *dYdX;
                    for(unit_t i=1;i<=N;++i)
                    {
                        Z[i] = compute(xp,i,X,Y,N, &dy[i]);
                    }
                }
                else
                {
                    for(unit_t i=1;i<=N;++i)
                    {
                        Z[i] = compute(xp,i,X,Y,N,0);
                    }
                }
            }


            //______________________________________________________________
            //
            // two passes smoothing
            // with derivative approximation and smoothing
            //______________________________________________________________
            inline void operator()(const expand<T> &xp,
                                   array<T>        &Z,
                                   const array<T>  &X,
                                   const array<T>  &Y,
                                   array<T>        &dYdX)
            {
                assert(X.size()==Y.size());
                assert(Y.size()==Z.size());
                assert(dYdX.size()==Y.size());
                assert(X.size()>0);

                const unit_t N(X.size());
                vector<T>    W(X.size());

                //______________________________________________________________
                //
                // first pass: zero^th order and derivative approx
                //______________________________________________________________
                for(unit_t i=1;i<=N;++i)
                {
                    Z[i] = compute(xp,i,X,Y,N,&W[i]);
                }


                //______________________________________________________________
                //
                // second pass: resample derivative
                //______________________________________________________________
                const expand<T> drvs(get_derivative_expand_type(xp.lower),
                                     get_derivative_expand_type(xp.upper));
                const size_t  old_degree = degree;
                if(degree>0) --degree;
                try
                {
                    for(unit_t i=1;i<=N;++i)
                    {
                        dYdX[i] = compute(drvs,i,X,W,N,0);
                    }
                    degree=old_degree;
                }
                catch (...)
                {
                    degree=old_degree;
                }
            }


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(smooth);
            inline void push_back(  T x, T y ) { const point_t p(x,y); points.push_back(p);  }
            inline void push_front( T x, T y ) { const point_t p(x,y); points.push_front(p); }

            matrix<T> mu;
            vector<T> coeff;

            inline void polynomial()
            {
                const size_t np = points.size();
                const size_t m  = min_of<size_t>(degree+1,np);

                //______________________________________________________________
                //
                // initialize parameters
                //______________________________________________________________
                mu.make(m);
                mu.ldz();
                coeff.make(m,0);
                typename points_t::iterator pp = points.begin();


                //______________________________________________________________
                //
                // accumulate
                //______________________________________________________________
                for(size_t i=np;i>0;--i,++pp)
                {
                    const point_t p = *pp;
                    for(size_t j=m,jm=m-1;j>0;--j,--jm)
                    {
                        const T   xjm  = ipower<T>(p.x,jm);
                        array<T> &mu_j = mu[j];

                        coeff[j]   += xjm * p.y;
                        for(size_t k=j,km=jm;k>0;--k,--km)
                        {
                            const T xkm = ipower<T>(p.x,km);
                            mu_j[k] += xkm*xjm;
                        }
                    }
                }

                //______________________________________________________________
                //
                // symetrisation
                //______________________________________________________________
                for(size_t j=m;j>0;--j)
                {
                    array<T> &mu_j = mu[j];
                    for(size_t k=j+1;k<=m;++k)
                    {
                        mu_j[k] = mu[k][j];
                    }
                }

                //______________________________________________________________
                //
                // compute parameters
                //______________________________________________________________
                if(!LU<T>::build(mu))
                {
                    throw libc::exception( EINVAL, "invalid data to smooth" );
                }
                LU<T>::solve(mu,coeff);
            }
            
        };
    }
}


#endif
