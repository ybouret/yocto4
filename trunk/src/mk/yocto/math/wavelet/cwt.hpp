#ifndef YOCTO_MATH_WAVELET_CWT_INCLUDED
#define YOCTO_MATH_WAVELET_CWT_INCLUDED 1

#include "yocto/math/kernel/matrix.hpp"
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

                // precompute scales
                for(size_t j=1;j<=n;++j)
                {
                    scales[j] =(j*width)/n;
                }

                // run
                for(size_t i=1;i<=n;++i)
                {
                    // shift
                    const T shift = x[i]-x[1];
                    shifts[i]  = shift;
                    for(size_t j=1;j<=n;++j)
                    {
                        const T scale = scales[j];
                        T sum(0);

                        for(size_t k=1,kp=2;k<n;++k,++kp)
                        {
                            const T xk  = x[k];
                            const T xkp = x[kp];
                            const T dx  = xkp - xk;
                            const T yk  = y[k];
                            const T ykp = y[kp];

                            sum += dx*(yk*Psi((xk-shift)/scale)+ykp*Psi((xkp-shift)/scale));
                        }

                        const T factor = Sqrt(scale);
                        sum /= factor+factor;

                        W[i][j] = sum;
                    }
                    
                }
            }
            
        };
        
    }
}

#endif
