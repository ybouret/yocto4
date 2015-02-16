#ifndef YOCTO_MATH_WAVELET_CWT_INCLUDED
#define YOCTO_MATH_WAVELET_CWT_INCLUDED 1

#include "yocto/math/kernel/matrix.hpp"
#include "yocto/math/types.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/math/opt/bracket.hpp"
#include "yocto/math/opt/minimize.hpp"

namespace yocto
{
    namespace math
    {

        template <typename T>
        struct wavelet
        {


            typedef typename numeric<T>::function function;


            //!
            /**
             \param x must be sorted
             \param y signal values @x
             */
            inline static
            void cwt(const array<T> &x,
                     const array<T> &y,
                     function       &Psi,
                     array<T>       &shifts,
                     array<T>       &scales,
                     matrix<T>      &W
                     )
            {
                assert(x.size()==y.size());
                assert(x.size()==shifts.size());
                assert(x.size()==scales.size());
                assert(x.size()>1);
                const size_t n        = x.size();
                const T      width    = x[n] - x[1]; assert(width>0);
                W.make(n,n);

                vector<T> psi(n,0);
                vector<T> ddx(n-1,0);
                // precompute scales and ddx
                for(size_t j=1;j<n;++j)
                {
                    scales[j] =(j*width)/n;
                    ddx[j] = x[j+1]-x[j];
                }
                scales[n] = width;

                // run
                for(size_t i=n;i>0;--i)
                {
                    // shift
                    const T shift = x[i]-x[1];
                    shifts[i]     = shift;

                    for(size_t j=n;j>0;--j)
                    {
                        const T scale = scales[j];
                        for(size_t k=n;k>0;--k)
                        {
                            psi[k] = Psi((x[k]-shift)/scale);
                        }
                        T sum(0);
                        for(size_t k=1,kp=2;k<n;++k,++kp)
                        {
                            const T dx  = ddx[k];
                            sum += dx*(y[k]*psi[k]+y[kp]*psi[kp]);
                        }

                        const T factor = Sqrt(scale);
                        sum /= factor+factor;

                        W[i][j] = sum;
                    }

                }
            }

            //! Mexican Hat
            static inline T Ricker( T x )
            {
                T x2 = x*x;
                return (T(1)-x2) * Exp(-x2/2);
            }

            //! Gaussian
            static inline T Gaussian(T x)
            {
                return Exp(-(x*x)/2);
            }



            struct cwt_opt_wrapper
            {
                const array<T> &x;
                const array<T> &y;
                array<T>       &z;
                function       &Psi;
                T               shift;  //!< the changing parameter
                const array<T> &scales;
                array<T>       &psi;
                const array<T> &ddx;

                inline T eval( T alpha )
                {
                    return cwt_power(x,y,z,Psi,shift,scales,alpha,psi,ddx);
                }
            };

            //!
            /**
             \param x must be sorted
             \param y signal values @x
             */
            inline static
            void cwt_opt(const array<T> &x,
                         const array<T> &y,
                         function       &Psi,
                         array<T>       &shifts,
                         array<T>       &scales,
                         matrix<T>      &W
                         )
            {
                assert(x.size()==y.size());
                assert(x.size()==shifts.size());
                assert(x.size()==scales.size());
                assert(x.size()>1);
                const size_t n        = x.size();
                const T      width    = x[n] - x[1]; assert(width>0);
                W.make(n,n);

                vector<T> psi(n,0);
                vector<T> ddx(n-1,0);
                vector<T> z(n,0);

                //______________________________________________________________
                //
                // precompute scales and ddx
                //______________________________________________________________
                for(size_t j=1;j<n;++j)
                {
                    scales[j] =(j*width)/n;
                    ddx[j] = x[j+1]-x[j];
                }
                scales[n] = width;

                //______________________________________________________________
                //
                // prepare approximated power estimator
                //______________________________________________________________
                cwt_opt_wrapper wrapper = { x, y, z, Psi, 0, scales, psi, ddx };
                function        F( &wrapper, &cwt_opt_wrapper::eval );

                
                //______________________________________________________________
                //
                // First time estimator
                //______________________________________________________________
                const T r = rms_of(x, y, ddx);
                triplet<T> alpha = { -r, 0, r };

                //______________________________________________________________
                //
                // run: for each shift
                //______________________________________________________________
                for(size_t i=1;i<=n;++i)
                {

                    //__________________________________________________________
                    //
                    // get shift
                    //__________________________________________________________
                    const T shift = x[i]-x[1];
                    shifts[i]     = shift;
                    wrapper.shift = shift;

                    //__________________________________________________________
                    //
                    // initialize look up
                    //__________________________________________________________
                    triplet<T> FF = { F(alpha.a), F(alpha.b), F(alpha.c) };
                    bracket<T>::expand(F,alpha,FF);
                    minimize<T>(F, alpha, FF, 0);

                    std::cerr << "shift=" << shift << "\t:\t@" << alpha.b << std::endl;

                    //__________________________________________________________
                    //
                    // compute transform for the given shift
                    //__________________________________________________________
                    for(size_t i=n;i>0;--i)
                    {
                        z[i] = y[i] + alpha.b;
                    }
                    array<T> &Wi = W[i];
                    for(size_t j=n;j>0;--j)
                    {
                        const T scale = scales[j];
                        for(size_t k=n;k>0;--k)
                        {
                            psi[k] = Psi((x[k]-shift)/scale);
                        }

                        T sum(0);
                        for(size_t k=1,kp=2;k<n;++k,++kp)
                        {
                            const T dx  = ddx[k];
                            sum += dx*(z[k]*psi[k]+z[kp]*psi[kp]);
                        }

                        const T factor = Sqrt(scale);
                        sum /= factor+factor;

                        Wi[j] = sum;
                    }
                }

            }

        private:
            static inline
            T rms_of( const array<T> &x, const array<T> &y, const array<T> &ddx ) throw()
            {
                assert(x.size()==y.size());
                assert(x.size()>=ddx.size());
                assert(x.size()>1);

                const size_t n = x.size();
                T r = 0;
                for(size_t i=1;i<n;++i)
                {
                    r += ddx[i] * ( Square(y[i]) + Square(y[i+1]));
                }
                const T width = x[n] - x[1];
                r /= width+width;
                return Sqrt(r);
            }

            static inline
            T cwt_power(const array<T> &x,
                        const array<T> &y,
                        array<T>       &z,
                        function       &Psi,
                        const T         shift,
                        const array<T> &scales,
                        const T         alpha,
                        array<T>       &psi,
                        const array<T> &ddx)
            {
                assert(x.size()==y.size());
                assert(x.size()==scales.size());
                assert(x.size()==psi.size());
                assert(x.size()>=ddx.size());
                assert(x.size()>1);

                const size_t n = x.size();
                T            F = 0;

                for(size_t i=n;i>0;--i)
                {
                    z[i] = y[i] + alpha;
                }

                for(size_t j=n;j>0;--j)
                {
                    const T scale = scales[j];
                    for(size_t k=n;k>0;--k)
                    {
                        psi[k] = Psi((x[k]-shift)/scale);
                    }
                    T sum(0);
                    for(size_t k=1,kp=2;k<n;++k,++kp)
                    {
                        const T dx  = ddx[k];
                        sum += dx*(z[k]*psi[k]+z[kp]*psi[kp]);
                    }

                    const T factor = Sqrt(scale);
                    sum /= factor+factor;

                    F += sum*sum;
                }
                return F;
            }

        };
        
    }
    
}

#endif
