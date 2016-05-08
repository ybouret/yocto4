#include "yocto/math/fft/fft1.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/math/types.hpp"
#include "yocto/code/unroll.hpp"
#include "yocto/code/xbitrev.hpp"

namespace yocto
{
    namespace math
    {


        template <>
        void Fourier<real_t>:: Transform(real_t      *data,
                                         const size_t size,
                                         const int    isign
                                         ) throw()
        {
            assert( data != NULL );
            assert( is_a_power_of_two(size) );
            assert( isign==1 || isign==-1);
            --data;

            //==================================================================
            // bit reversal algorithm
            //==================================================================
            const size_t n    = size << 1;
            const real_t coef = REAL(1.0)/ Sqrt(  real_t(size) );
            xbitrev::run(data,size);
            
            //==================================================================
            // Lanczos-Algorithm
            //==================================================================
            {
                const real_t sgn_two_pi = isign * numeric<real_t>::two_pi;
                size_t       mmax       = 2;
                while (n > mmax)
                {
                    const size_t istep = mmax << 1;
                    const real_t theta = sgn_two_pi/mmax;
                    real_t wtemp       = Sin(REAL(0.5)*theta);
                    real_t wpr         = -REAL(2.0)*wtemp*wtemp;
                    real_t wpi         = Sin(theta);
                    real_t wr          = REAL(1.0);
                    real_t wi          = REAL(0.0);

                    for (size_t m=1;m<mmax;m+=2)
                    {
                        for (size_t i=m;i<=n;i+=istep)
                        {
                            real_t      *d_i   = data+i;
                            size_t       j     = i+mmax;
                            real_t      *d_j   = data+j;
                            const real_t tempr = wr*d_j[0]-wi*d_j[1];
                            const real_t tempi = wr*d_j[1]+wi*d_j[0];

                            d_j[0]  = d_i[0]-tempr;
                            d_j[1]  = d_i[1]-tempi;
                            d_i[0] += tempr;
                            d_i[1] += tempi;
                        }
                        wr=(wtemp=wr)*wpr-wi*wpi+wr;
                        wi=wi*wpr+wtemp*wpi+wi;
                    }
                    mmax=istep;
                }
            }
            
            //==================================================================
            // Normalizing
            //==================================================================
#define _YOCTO_FFT_SCALE(INDEX) data[INDEX] *= coef
            YOCTO_LOOP_FUNC_(n,_YOCTO_FFT_SCALE,1);
        }
        
        
        template <>
        void Fourier<real_t>::  Transform( const real_t *data1, const real_t *data2, real_t *fft1, real_t *fft2, const size_t n) throw()
        {
            assert( data1 != NULL );
            assert( data2 != NULL );
            assert( fft1  != NULL );
            assert( fft2  != NULL );
            
            assert( is_a_power_of_two(n) );

            //__________________________________________________________________
            //
            // packing real data in fft1
            //__________________________________________________________________
            for(size_t j=0,jj=1;j<n;j++,jj+=2)
            {
                fft1[jj-1]=data1[j];
                fft1[jj]  =data2[j];
            }

            //__________________________________________________________________
            //
            // forward transform complex fft1
            //__________________________________________________________________
            Transform(fft1,n,1);

            //__________________________________________________________________
            //
            // decompose fourier transforms
            //__________________________________________________________________
            const size_t np1 = n+1;
            const size_t nn2 = np1<<1;
            const size_t nn3 = nn2+1;
            --fft1;
            --fft2;
            fft2[1]=fft1[2];
            fft1[2]=fft2[2]=REAL(0.0);
            for(size_t j=3;j<=np1;j+=2)
            {
                const size_t jp1= j+1;
                const real_t rep=REAL(0.5)*(fft1[j]   + fft1[nn2-j]);
                const real_t rem=REAL(0.5)*(fft1[j]   - fft1[nn2-j]);
                const real_t aip=REAL(0.5)*(fft1[jp1] + fft1[nn3-j]);
                const real_t aim=REAL(0.5)*(fft1[jp1] - fft1[nn3-j]);
                fft1[j]     =  rep;
                fft1[jp1]   =  aim;
                fft1[nn2-j] =  rep;
                fft1[nn3-j] = -aim;
                fft2[j]     =  aip;
                fft2[jp1]   = -rem;
                fft2[nn2-j] =  aip;
                fft2[nn3-j] =  rem;
            }
        }
        
    }
}
