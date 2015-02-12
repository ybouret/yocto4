#ifndef YOCTO_MATH_WAVELET_CWT_INCLUDED
#define YOCTO_MATH_WAVELET_CWT_INCLUDED 1

#include "yocto/math/kernel/matrix.hpp"
#include "yocto/math/types.hpp"

namespace yocto
{
    namespace math
    {

        namespace wavelet
        {

            //!
            /**
             \param x must be sorted
             \param y functions values @x
             */
            template <typename T>
            inline void cwt(const array<T> &x,
                            const array<T> &y,
                            typename numeric<T>::function &Psi,
                            matrix<T>      &W
                            )
            {
                assert(x.size()==y.size());
                assert(x.size()>1);
                const size_t n = x.size();
                const T      width    = x[n] - x[1]; assert(width>0);
                const T      freq_max = (n-1)/width;
                W.make(n,n);
                for(size_t i=1;i<=n;++i)
                {
                    // shift
                    const T s = x[i]-x[1];

                    for(size_t j=1;j<=n;++j)
                    {
                        const T omega = (j*freq_max)/n;

                        T sum(0);

                        for(size_t k=1;j<n;++k)
                        {
                            const T xk  = x[k];
                            const T xkp = x[k+1];
                            const T dx  = xkp - xk;
                            sum += dx*(y[k]*Psi((xk-s)/omega)+y[k+1]*Psi((xkp-s)/omega));
                        }

                        sum /= 2*Sqrt(omega);

                        W[i][j] = sum;
                    }

                }
            }

        }
    }
}

#endif
