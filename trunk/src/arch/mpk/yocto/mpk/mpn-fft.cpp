#include "yocto/mpk/natural.hpp"

#include "yocto/math/types.hpp"

#include "yocto/memory/global.hpp"

#include "yocto/code/unroll.hpp"
#include "yocto/code/bswap.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/code/bswap.hpp"

namespace yocto
{
    using namespace math;
	
    namespace mpk
    {
        typedef double          real_t;
        typedef complex<real_t> cplx_t;
		
        //! simultaneous FFTs
		static inline
        void _xfft(real_t      *data,
                   real_t      *other,
                   const size_t size
                   ) throw()
        {
            assert( data  != NULL );
			assert( other != NULL );
            assert( is_a_power_of_two(size) );
			
            --data;
			--other;
			
            //==================================================================
            // bit reversal algorithm
            //==================================================================
            const size_t n    = size << 1;
            {
                size_t j=1;
                for(size_t i=1; i<n; i+=2)
                {
                    if(j>i)
					{
                        core::bswap<2*sizeof(real_t)>( data+i,  data+j );
                        core::bswap<2*sizeof(real_t)>( other+i, other+j);
                    }
                    size_t m = size; // m=  n / 2;
                    while (m >= 2 && j > m)
					{
                        j -= m;
                        m >>= 1;
                    }
                    j += m;
                }
            }
			
            //==================================================================
            // Lanczos-Algorithm
            //==================================================================
            {
                static const real_t sgn_two_pi = numeric<real_t>::two_pi;
                size_t              mmax       = 2;
                while (n > mmax)
                {
                    const size_t istep = mmax << 1;
                    const real_t theta = sgn_two_pi/mmax;
                    real_t wtemp       = sin(0.5*theta);
                    real_t wpr         = - 2.0*wtemp*wtemp;
                    real_t wpi         = sin(theta);
                    real_t wr          = 1.0;
                    real_t wi          = 0.0;
					
                    for (size_t m=1; m<mmax; m+=2)
					{
                        for (size_t i=m; i<=n; i+=istep)
						{
							const size_t       j     = i+mmax;
							
							{
								real_t      *d_i   = data+i;
								real_t      *d_j   = data+j;
								const real_t tempr = wr*d_j[0]-wi*d_j[1];
								const real_t tempi = wr*d_j[1]+wi*d_j[0];
								
								d_j[0]  = d_i[0]-tempr;
								d_j[1]  = d_i[1]-tempi;
								d_i[0] += tempr;
								d_i[1] += tempi;
							}
							
							{
								real_t      *o_i   = other+i;
								real_t      *o_j   = other+j;
								const real_t tempr = wr*o_j[0]-wi*o_j[1];
								const real_t tempi = wr*o_j[1]+wi*o_j[0];
								
								o_j[0]  = o_i[0]-tempr;
								o_j[1]  = o_i[1]-tempi;
								o_i[0] += tempr;
								o_i[1] += tempi;
							}
                        }
                        wr=(wtemp=wr)*wpr-wi*wpi+wr;
                        wi=wi*wpr+wtemp*wpi+wi;
                    }
                    mmax=istep;
                }
            }
			
			
        }
		
		static inline
		void _fft(real_t      *data,
				  const size_t size
				  ) throw()
        {
            assert( data != NULL );
            assert( is_a_power_of_two(size) );
			
            --data;
			
            //==================================================================
            // bit reversal algorithm
            //==================================================================
            const size_t n = size << 1;
            {
                size_t j=1;
                for (size_t i=1; i<n; i+=2) {
                    if (j > i)
                    {
                        core::bswap<2*sizeof(real_t)>( data+i, data+j);
                    }
                    size_t m = size; // m=  n / 2;
                    while (m >= 2 && j > m) {
                        j -= m;
                        m >>= 1;
                    }
                    j += m;
                }
            }
			
            //==================================================================
            // Lanczos-Algorithm
            //==================================================================
            {
                static const real_t sgn_two_pi = numeric<real_t>::two_pi;
                size_t              mmax       = 2;
                while (n > mmax) {
                    const size_t istep = mmax << 1;
                    const real_t theta = sgn_two_pi/mmax;
                    real_t wtemp       = sin(0.5*theta);
                    real_t wpr         = - 2.0*wtemp*wtemp;
                    real_t wpi         = sin(theta);
                    real_t wr          = 1.0;
                    real_t wi          = 0.0;
					
                    for (size_t m=1; m<mmax; m+=2)
                    {
                        for (size_t i=m; i<=n; i+=istep)
                        {
                            real_t      *d_i   = data+i;
                            const size_t j     = i+mmax;
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
			
			
        }
		
		
		static  inline
		void _ifft(real_t      *data,
				   const size_t size
				   ) throw()
        {
            assert( data != NULL );
            assert( is_a_power_of_two(size) );
			
            --data;
			
            //==================================================================
            // bit reversal algorithm
            //==================================================================
            const size_t n = size << 1;
            {
                size_t j=1;
                for (size_t i=1; i<n; i+=2) {
                    if (j > i)
                    {
                        core::bswap<2*sizeof(real_t)>( data+i, data+j);
                    }
                    size_t m = size; // m=  n / 2;
                    while (m >= 2 && j > m) {
                        j -= m;
                        m >>= 1;
                    }
                    j += m;
                }
            }
			
            //==================================================================
            // Lanczos-Algorithm
            //==================================================================
            {
                static const real_t sgn_two_pi = -numeric<real_t>::two_pi;
                size_t              mmax       = 2;
                while (n > mmax) {
                    const size_t istep = mmax << 1;
                    const real_t theta = sgn_two_pi/mmax;
                    real_t wtemp       = sin(0.5*theta);
                    real_t wpr         = - 2.0*wtemp*wtemp;
                    real_t wpi         = sin(theta);
                    real_t wr          = 1.0;
                    real_t wi          = 0.0;
					
                    for (size_t m=1; m<mmax; m+=2) {
                        for (size_t i=m; i<=n; i+=istep) {
                            real_t      *d_i   = data+i;
                            const size_t j     = i+mmax;
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
			
			
        }
		
		
#define YMPK_FFT_UNROLL 1
        natural natural:: fft_( const natural &lhs, const natural &rhs )
        {
            const size_t nL = lhs.size_;
            const size_t nR = rhs.size_;
            if( nL >0 && nR > 0 )
			{
                const size_t nP = nL + nR;             //-- product size
                natural       P( nP, as_capacity );    //-- product value
				
				//--------------------------------------------------------------
                //-- compute common power of two
				//--------------------------------------------------------------
				const size_t nN = next_power_of_two(nP);
                
				//--------------------------------------------------------------
                //- compute wokspaces
				//--------------------------------------------------------------
				array_of<cplx_t> L(nN);
				array_of<cplx_t> R(nN);
                const uint8_t *l      = lhs.byte_;
                const uint8_t *r      = rhs.byte_;
				
				//--------------------------------------------------------------
                //-- fill workspaces
				//--------------------------------------------------------------
#define         YMPK_FFT_L(J) L[J].re = real_t( l[J] )
                YOCTO_LOOP_FUNC_(nL,YMPK_FFT_L,0);
				
#define         YMPK_FFT_R(J) R[J].re  = real_t( r[J] )
                YOCTO_LOOP_FUNC_(nR,YMPK_FFT_R,0);
				
				//--------------------------------------------------------------
                //-- forward
				//--------------------------------------------------------------
                _xfft( & L[0].re, & R[0].re, nN   );
				
				//--------------------------------------------------------------
                //-- multiply in place, in L
				//--------------------------------------------------------------
#define         YMPK_FFT_P(J) L[J] *= R[J]
                YOCTO_LOOP_FUNC_(nN,YMPK_FFT_P,0);
				
				//--------------------------------------------------------------
                //-- reverse
				//--------------------------------------------------------------
                _ifft( & L[0].re, nN );
				
                real_t       carry = 0.0;
                uint8_t     *prod  = P.byte_;
                const size_t top   = nP - 1;
				
                static const real_t half = 0.5;
                
#if YMPK_FFT_UNROLL == 1
#   define YMPK_FFT_U(J)                               \
/**/  carry += L[J].re/nN + half;                      \
/**/  const real_t _q = real_t(size_t( carry/256.0 )); \
/**/  const real_t _r = carry - 256.0 * _q;            \
/**/  prod[J]        = uint8_t(_r);                    \
/**/  carry          = _q
				
                YOCTO_LOOP_FUNC_(top,YMPK_FFT_U,0);
#else
                for( size_t i=0; i < top; ++i ) {
                    carry         +=  L[i].re/nN + half;
                    const real_t q = size_t( carry / 256.0 );
                    const real_t r = carry - 256.0 * q;
                    prod[i]   = uint8_t(r);
                    carry = q;
                }
#endif
                prod[top] = uint8_t(carry);
                
                P.update();
                return P;
            }
            else
                return natural();
        }
		
		
		natural natural:: sqr_( const natural &lhs )
        {
            const size_t nL = lhs.size_;
            if( nL > 0  )
			{
                const size_t nP = nL << 1;             //-- product size
                natural       P( nP, as_capacity );    //-- product value
				//--------------------------------------------------------------
                //-- compute power of two
				//--------------------------------------------------------------
                const size_t nN = next_power_of_two(nP);
                
				//--------------------------------------------------------------
                //- compute wokspace size and create it
				//--------------------------------------------------------------
                array_of<cplx_t> L( nN );
                const uint8_t *l      = lhs.byte_;
				
				//--------------------------------------------------------------
                //-- fill workspaces
				//--------------------------------------------------------------
#undef YMPK_FFT_L
#define         YMPK_FFT_L(J) L[J].re = real_t( l[J] )
                YOCTO_LOOP_FUNC_(nL,YMPK_FFT_L,0);
				
				//--------------------------------------------------------------
                //-- forward
				//--------------------------------------------------------------
                _fft( & L[0].re,  nN   );
				
				//--------------------------------------------------------------
                //-- multiply in place, in L
				//--------------------------------------------------------------
#undef YMPK_FFT_P
#define         YMPK_FFT_P(J) L[J] *= L[J]
                YOCTO_LOOP_FUNC_(nN,YMPK_FFT_P,0);
				
				//--------------------------------------------------------------
                //-- reverse
				//--------------------------------------------------------------
                _ifft( & L[0].re, nN );
				
                real_t       carry = 0;
                uint8_t     *prod  = P.byte_;
                const size_t top   = nP - 1;
				
#if YMPK_FFT_UNROLL == 1
#	undef  YMPK_FFT_U
#   define YMPK_FFT_U(J)                             \
/**/  carry += L[J].re/nN + 0.5;                     \
/**/  const real_t q = real_t(size_t( carry/256.0 ));\
/**/  const real_t r = carry - 256.0 * q;            \
/**/  prod[J]        = uint8_t(r);                   \
/**/  carry          = q
				
                YOCTO_LOOP_FUNC_(top,YMPK_FFT_U,0);
#else
                for( size_t i=0; i < top; ++i ) {
                    carry         +=  L[i].re/nN + 0.5;
                    const real_t q = size_t( carry / 256.0 );
                    const real_t r = carry - 256.0 * q;
                    prod[i]   = uint8_t(r);
                    carry = q;
                }
#endif
                prod[top] = uint8_t(carry);
				
                P.update();
                return P;
            } else
                return natural();
        }
		
		
    }
	
}
