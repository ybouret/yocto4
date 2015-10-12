#ifndef YOCTO_MATH_WAVELET_CWT_INCLUDED
#define YOCTO_MATH_WAVELET_CWT_INCLUDED 1

#include "yocto/math/matrix.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/math/types.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/code/unroll.hpp"

namespace yocto
{
    namespace math
    {

        template <typename T>
        struct wavelet
        {


            typedef typename numeric<T>::function function;



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


            //!
            /**
             \param x must be sorted
             \param y signal values @x
             \param Psi the mother function
             \param shifts the shifts values
             \param scales the scales values
             \param W the matrix of coefficients W[shift][scale]
             */
            inline static
            T cwt(const array<T> &x,
                  const array<T> &y,
                  function       &Psi,
                  array<T>       &shifts,
                  array<T>       &scales,
                  matrix<T>      &W,
                  const bool      optimize = true
                  )
            {
                assert(x.size()==y.size());
                assert(x.size()==shifts.size());
                assert(x.size()==scales.size());
                assert(x.size()>1);
                //______________________________________________________________
                //
                // constant parameters
                //______________________________________________________________
                const size_t n        = x.size();
                const T      x1       = x[1];
                const T      width    = x[n] - x1; assert(width>0);
                W.make(n,n);

                //______________________________________________________________
                //
                // local memory
                //______________________________________________________________
                vector<T> ddx(n-1,0);
                vector<T> z(n,0);

                //______________________________________________________________
                //
                // precompute scales, shifts and ddx
                //______________________________________________________________
                for(size_t j=1,jp=2;j<n;++j,++jp)
                {
                    scales[j]  = (j*width)/n;
                    const T xj = x[j];
                    ddx[j]     = x[jp]-xj;
                    shifts[j]  = xj-x1;
                }
                scales[n] = width;
                shifts[n] = width;

                T alpha(0);
                if(optimize)
                {

                    //__________________________________________________________
                    //
                    // Compute global baseline coefficient
                    //__________________________________________________________
                    T Ifull(0), Jfull(0);

                    //__________________________________________________________
                    //
                    // Loop over shifts
                    //__________________________________________________________
                    for(size_t i=n;i>0;--i)
                    {
                        const T shift = shifts[i];

                        //______________________________________________________
                        //
                        // Loop over scales
                        //______________________________________________________
                        for(size_t j=n;j>0;--j)
                        {
                            const T scale   = scales[j];

                            T sum_psi(0); //! integral w.r.t x
                            T sum_wav(0); //! integral w.r.t x
                            T psi_k  = Psi( (x1-shift)/scale );
                            for(size_t k=1,kp=2;k<n;++k,++kp)
                            {
                                const T psi_kp = Psi( (x[kp]-shift)/scale );
                                const T dx     = ddx[k];

                                sum_psi += dx * (psi_k+psi_kp);
                                sum_wav += dx * (psi_k*y[k]+psi_kp*y[kp]);

                                psi_k = psi_kp;
                            }

                            Ifull += (sum_psi*sum_psi)/scale;
                            Jfull += (sum_psi*sum_wav)/scale;
                        }
                    }
                    alpha = -Jfull/Ifull;
                }

                //______________________________________________________________
                //
                // Generate Data
                //______________________________________________________________
#define Y_CWT_MAKE_Z(I) z[I] = y[I] + alpha
                YOCTO_LOOP_FUNC_(n,Y_CWT_MAKE_Z,1);
#undef  Y_CWT_MAKE_Z


                //______________________________________________________________
                //
                // Loop...
                //______________________________________________________________
                for(size_t i=n;i>0;--i)
                {
                    const T   shift = shifts[i];
                    array<T> &Wi    = W[i];

                    for(size_t j=n;j>0;--j)
                    {
                        const T         scale = scales[j];
                        T sum(0);
                        T psi_k  = Psi( (x1-shift)/scale );
                        
                        for(size_t k=1,kp=2;k<n;++k,++kp)
                        {
                            const T psi_kp = Psi( (x[kp]-shift)/scale );
                            sum += ddx[k]*(z[k]*psi_k+z[kp]*psi_kp);
                            psi_k = psi_kp;
                        }

                        {
                            const T factor = Sqrt(scale);
                            sum /= factor+factor;
                        }

                        Wi[j] = sum;
                    }
                }

                return alpha;
            }

            //! normalize the matrix/amplitude
            /**
             - rescale by the the max of |negative| or |positive| value
             */
            static inline void rescale( matrix<T> &W ) throw()
            {
                const size_t n=W.rows;
                const size_t m=W.cols;
                if(n<=0) return;
                T lo = W[1][1];
                T hi = lo;
                for(size_t i=n;i>0;--i)
                {
                    const array<T> &Wi = W[i];
                    for(size_t j=m;j>0;--j)
                    {
                        const T tmp = Wi[j];
                        if(tmp<lo)
                        {
                            lo=tmp;
                        }
                        if(tmp>hi)
                        {
                            hi=tmp;
                        }
                    }
                }

                const T amplitude = hi-lo;

                if(amplitude>0)
                {
                    const T den = (hi>=lo ? hi : -lo);
                    for(size_t i=n;i>0;--i)
                    {
                        array<T> &Wi = W[i];
                        for(size_t j=m;j>0;--j)
                        {
                            const T w = clamp<T>(-1,Wi[j]/den,1);
                            Wi[j] = w;
                        }
                    }
                }
                
                
            }
            
        private:
            
        };
        
    }
    
}

#endif
