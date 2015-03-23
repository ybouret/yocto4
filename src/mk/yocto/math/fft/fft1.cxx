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

#if 0
            {
				size_t j=1;
				for (size_t i=1;i<n;i+=2) 
				{ 
					if(j>i) 
					{ 						
                        core::bswap<2*sizeof(real_t)>( data+i, data+j);
					}
					size_t m = size; // m=  n / 2;
					while( m>=2 && j>m )
					{
						j -= m;
						m >>= 1;
					}
					j += m;
				}
			}
#else
            xbitrev::run(data,size);
#endif
			
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
		
		
		
		
	}
}
