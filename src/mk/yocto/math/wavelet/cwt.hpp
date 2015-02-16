#ifndef YOCTO_MATH_WAVELET_CWT_INCLUDED
#define YOCTO_MATH_WAVELET_CWT_INCLUDED 1

#include "yocto/math/kernel/matrix.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/math/types.hpp"

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
            void cwt_raw(const array<T> &x,
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
                const T      x1       = x[1];
                const T      width    = x[n] - x1; assert(width>0);
                W.make(n,n);

                // local memory
                vector<T> ddx(n-1,0);
                vector<T> z(n,0);
                matrix<T> psi(n,n);

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

                //______________________________________________________________
                //
                // Loop on shifts
                //______________________________________________________________
                for(size_t i=1;i<=n;++i)
                {
                    const T shift = shifts[i];

                    //__________________________________________________________
                    //
                    // compute normalizing coef, store Psi values
                    //__________________________________________________________
                    T I(0), J(0);
                    for(size_t j=n;j>0;--j)
                    {
                        const T scale   = scales[j];
                        array<T> &psi_j = psi[j];
                        T sum_psi(0), sum_wav(0);
                        T psi_k  = Psi( (x1-shift)/scale );
                        psi_j[1] = psi_k;
                        for(size_t k=1,kp=2;k<n;++k,++kp)
                        {
                            const T psi_kp = Psi( (x[kp]-shift)/scale );
                            const T dx     = ddx[k];

                            psi_j[kp]      = psi_kp;
                            sum_psi += dx * (psi_k+psi_kp);
                            sum_wav += dx * (psi_k*y[k]+psi_kp*y[kp]);

                            psi_k = psi_kp;
                        }

                        I += sum_psi * sum_psi;
                        J += sum_psi * sum_wav;
                    }
                    const T alpha = -J/I;

                    //__________________________________________________________
                    //
                    // compute optimized transform
                    //__________________________________________________________
                    for(size_t j=n;j>0;--j)
                    {
                        z[j] = y[j] + alpha;
                    }

                    array<T> &Wi = W[i];
                    for(size_t j=n;j>0;--j)
                    {
                        const T         scale = scales[j];
                        const array<T> &psi_j = psi[j];
                        T sum(0);
                        T psi_k  = psi_j[1];
                        for(size_t k=1,kp=2;k<n;++k,++kp)
                        {
                            const T psi_kp = psi_j[kp];
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
            }

            //!
            /**
             \param x must be sorted
             \param y signal values @x
             */
            inline static
            void cwt_full(const array<T> &x,
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
                const T      x1       = x[1];
                const T      width    = x[n] - x1; assert(width>0);
                W.make(n,n);

                // local memory
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

                //______________________________________________________________
                //
                // Compute global baseline coefficient
                //______________________________________________________________
                T Ifull(0), Jfull(0);

                {
                    //__________________________________________________________
                    //
                    // Loop on shifts
                    //__________________________________________________________
                    for(size_t i=n;i>0;--i)
                    {
                        const T shift = shifts[i];

                        //______________________________________________________
                        //
                        // compute normalizing coef, store Psi values
                        //______________________________________________________
                        T I(0), J(0);
                        for(size_t j=n;j>0;--j)
                        {
                            const T scale   = scales[j];
                            T sum_psi(0), sum_wav(0);
                            T psi_k  = Psi( (x1-shift)/scale );
                            for(size_t k=1,kp=2;k<n;++k,++kp)
                            {
                                const T psi_kp = Psi( (x[kp]-shift)/scale );
                                const T dx     = ddx[k];

                                sum_psi += dx * (psi_k+psi_kp);
                                sum_wav += dx * (psi_k*y[k]+psi_kp*y[kp]);

                                psi_k = psi_kp;
                            }

                            I += sum_psi * sum_psi;
                            J += sum_psi * sum_wav;
                            Ifull += I/scale;
                            Jfull += J/scale;
                        }
                    }
                }
                const double alpha = -Jfull/Ifull;
                std::cerr << "alpha_full=" << alpha << std::endl;

                //______________________________________________________________
                //
                // Generate global data
                //______________________________________________________________
                for(size_t i=n;i>0;--i)
                {
                    z[i] = y[i] + alpha;
                }

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
                
            }
            
        private:
            
        };
        
    }
    
}

#endif
