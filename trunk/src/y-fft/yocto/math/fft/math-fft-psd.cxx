#include "yocto/math/ztype.hpp"
#include "yocto/math/types.hpp"
#include "yocto/math/fft/psd.hpp"
#include "yocto/math/fft/fft1.hpp"

#include "yocto/code/utils.hpp"
#include "yocto/code/unroll.hpp"

#include "yocto/exceptions.hpp"
#include "yocto/sequence/vector.hpp"

#include <cerrno>

namespace yocto
{
	
    namespace math
    {
		
        //======================================================================
        template <>
        real_t PSD<real_t>:: Square( real_t X )
        {
            return X>=REAL(0.0) && X<=REAL(1.0) ? REAL(1.0) : REAL(0.0);
        }
		
        template <>
        real_t PSD<real_t>:: Bartlett( real_t X )
        {
            return X>REAL(0.0) && X<REAL(1.0) ? REAL(1.0) - Fabs( (X-REAL(0.5) / REAL(0.5) ) ): REAL(0.0);
        }
		
        template <>
        real_t PSD<real_t>:: Hann( real_t X )
        {
            return X>REAL(0.0) && X<REAL(1.0) ? REAL(0.5) * (REAL(1.0) - Cos( numeric<real_t>::two_pi * X)) : REAL(0.0) ;
        }
		
        template <>
        real_t PSD<real_t>:: Welch( real_t X )
        {
            if(  X>0 && X<1 )
            {
                const real_t tmp = (X-REAL(0.5) / REAL(0.5));
                return REAL(1.0) - tmp * tmp;
            }
            else
            {
                return 0;
            }
			
        }
		
        //======================================================================
        template<>
        PSD<real_t>:: Gaussian:: Gaussian( real_t sig ) throw() : sigma( sig ) {}
		
        template<>
        PSD<real_t>:: Gaussian:: Gaussian( const Gaussian &G ) throw() : sigma( G.sigma ) {}
		
        template<>
        PSD<real_t>:: Gaussian:: ~Gaussian() throw() {}
		
        template<>
        real_t PSD<real_t>:: Gaussian:: operator()( real_t X ) const
        {
            if( X > 0 && X < 1 )
            {
                const real_t tmp = ((X-REAL(0.5) / REAL(0.5)))/sigma;
                return       Exp( - REAL(0.5) * tmp * tmp );
            }
            else {
                return 0;
            }
        }
		
        //======================================================================
        template<>
        PSD<real_t>:: Blackman:: Blackman( real_t  alpha  ) throw() :
		a0( (REAL(1.0)-alpha)*REAL(0.5) ),
		a1( REAL(0.5)),
		a2( REAL(0.5) * alpha)
        {}
		
        template<>
        PSD<real_t>:: Blackman:: Blackman( const Blackman &B ) throw() :
		a0( B.a0 ),
		a1( B.a1 ),
		a2( B.a2 )
        {}
		
        template<>
        PSD<real_t>:: Blackman:: ~Blackman() throw() {}
		
        template<>
        real_t PSD<real_t>:: Blackman:: operator()( real_t X ) const
        {
            if( X > 0 && X < 1 )
            {
                const real_t tmp  = numeric<real_t>::two_pi * X;
                return a0 - a1 * Cos( tmp  ) + a2 * Cos( tmp + tmp );
            }
            else {
                return 0;
            }
        }
		
		
		
#define YOCTO_PSD_INITIALIZE(INDEX)       p[INDEX] = 0
#define YOCTO_PSD_COMPUTE_WEIGHT(INDEX)   const real_t tmp = weight[INDEX] = w( INDEX * wfact ); sumw2 += tmp*tmp
#define YOCTO_PSD_COPY_INDEPENDENT(INDEX) input[INDEX] = cplx_t(start[INDEX] * weight[INDEX])
#define YOCTO_PSD_COPY_OVERLAPPING(INDEX) input[INDEX] = cplx_t(data[ (idx++) & mask ] * weight[INDEX])
		
#define YOCTO_PSD_ADD(INDEX) p[INDEX] += (input[INDEX].mod2())
		
#define YOCTO_PSD_NRM(INDEX) p[INDEX] *= pfact
		
        template<>
        size_t PSD<real_t>::Compute(Window       &w,
                                    real_t       *p,
                                    const size_t  m,
                                    const real_t *data,
                                    const size_t  size,
                                    const size_t  options)
        {
			const cplx_t cplx_zero(0);
            assert( p    != NULL );
            assert( data != NULL );
            assert( is_a_power_of_two(size) );
            assert( is_a_power_of_two(m)    );
			
			const bool   overlap   = (PSD_Overlap   & options) != 0;
            const size_t M         = m << 1;
            if( M > size )
                throw libc::exception(EDOM,"math::PowerSpectralDensity(2*m=%u>#data=%u)", unsigned(M), unsigned(size));
			
            YOCTO_LOOP_FUNC_(m,YOCTO_PSD_INITIALIZE,0);
            const size_t   shift = overlap ? m : M;
            const size_t   nsubs = size/shift;
            
			vector<cplx_t> work( M, cplx_zero );
            cplx_t        *input = work(0);
			vector<real_t> work2(M,0);
			real_t        *weight = work2(0);
			real_t         sumw2  = 0;
			{
				const real_t   wfact = REAL(1.0)/(M-1);
				YOCTO_LOOP_FUNC_(M,YOCTO_PSD_COMPUTE_WEIGHT,0);
			}
			
			
			
			if( overlap )
			{
				const size_t mask = size - 1;
				size_t       idx0 = 0;
				for( size_t k=0; k < nsubs; ++k, idx0 += shift )
				{
					//-- initialize input :
					//-- cyclic weighted copy
					size_t idx = idx0;
					YOCTO_LOOP_FUNC_(M,YOCTO_PSD_COPY_OVERLAPPING,0);
					
					//-- Fourier Transform
					FFT( input, M );
					
					//-- gather the spectrum from [0..m-1]
					YOCTO_LOOP_FUNC_(m,YOCTO_PSD_ADD,0);
				}
			}
			else
			{
				const real_t  *start = data;
				for(size_t k=0;
					k < nsubs;
					++k, start += shift )
				{
					//-- initialize input :
					//-- copy start[0..M-1] into input, weighted
					YOCTO_LOOP_FUNC_(M,YOCTO_PSD_COPY_INDEPENDENT,0);
					
					//-- Fourier Transform
					FFT( input, M );
					
					//-- gather the spectrum from [0..m-1]
					YOCTO_LOOP_FUNC_(m,YOCTO_PSD_ADD,0);
				}
				
			}
			
			//! Partial Normalization
			const real_t pfact = REAL(1.0) / (sumw2*nsubs);
			YOCTO_LOOP_FUNC_(m,YOCTO_PSD_NRM,0);
			
            return M;
        }
        
        template<>
        size_t PSD<real_t>::Compute(Window              &w,
                                    array<real_t>       &psd,
                                    const array<real_t> &data,
                                    const size_t         options)
        {
            
            return PSD<real_t>::Compute(w, psd(), psd.size(), data(), data.size(), options);
        }
		
    }
	
}
