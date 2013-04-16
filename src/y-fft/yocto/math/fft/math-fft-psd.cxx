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
        real_t PSD<real_t>:: Square( real_t X ) throw()
        {
            return X>=REAL(0.0) && X<=REAL(1.0) ? REAL(1.0) : REAL(0.0);
        }
		
        template <>
        real_t PSD<real_t>:: Bartlett( real_t X ) throw()
        {
            return X>REAL(0.0) && X<REAL(1.0) ? REAL(1.0) - Fabs( (X-REAL(0.5) / REAL(0.5) ) ): REAL(0.0);
        }
		
        template <>
        real_t PSD<real_t>:: Hann( real_t X ) throw()
        {
            return X>REAL(0.0) && X<REAL(1.0) ? REAL(0.5) * (REAL(1.0) - Cos( numeric<real_t>::two_pi * X)) : REAL(0.0) ;
        }
		
        template <>
        real_t PSD<real_t>:: Welch( real_t X ) throw()
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
        real_t PSD<real_t>:: Blackman( real_t X ) throw()
        {
            if( X > 0 && X < 1 )
            {
                const real_t tmp  = numeric<real_t>::two_pi * X;
                return REAL(0.42) - REAL(0.50) * Cos( tmp  ) + REAL(0.08) * Cos( tmp + tmp );
            }
            else
            {
                return 0;
            }
        }
        
        template <>
        real_t PSD<real_t>:: Nutall( real_t X ) throw()
        {
            if( X > 0 && X < 1 )
            {
                const real_t pi2 = numeric<real_t>:: two_pi * X;
                const real_t pi4 = pi2+pi2;
                const real_t pi6 = pi4+pi2;
                return REAL(0.355768) - REAL(0.487396) * Cos(pi2) + REAL(0.144232) * Cos(pi4) - REAL(0.012604) * Cos(pi6);
            }
            else
                return 0;
        }
		
		
#if 0
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
#endif
        
        template <>
        void PSD<real_t>:: Compute(Window        &w,
                                   real_t        *p,
                                   const size_t   m,
                                   const real_t *data,
                                   const size_t  size)
        {
            
            assert(p!=NULL);
            assert(m>0);
            assert(is_a_power_of_two(m));
            assert(!(data==NULL&&size>0));
            
            //==================================================================
            // initialize data
            //==================================================================
            const size_t M = m << 1;
            if( M > size )
                throw exception("PSD: %u*2 > data size=%u", unsigned(m), unsigned(size));
            
            
            vector<cplx_t> work(  M, numeric<cplx_t>::zero );
            vector<real_t> work2( M, numeric<real_t>::zero );
            cplx_t        *input  = work(0);
            real_t        *weight = work2(0);
            
            //==================================================================
            // initialize the weights
            //==================================================================
            real_t         sumw2  = 0;
            {
                const real_t wfac = REAL(1.0) / (M-1);
                for( size_t i=0; i < M; ++i )
                {
                    const real_t tmp = weight[i] = w( i * wfac ); // or (i+0.5)/M ?
                    sumw2 += tmp*tmp;
                }
            }
            
            //==================================================================
            // initialize the spectrum
            //==================================================================
            for(size_t i=0;i<m;++i)
                p[i] = 0;
            
            //==================================================================
            // loop over segments
            //==================================================================
            size_t nsub = 0;
            for( size_t start=0; start+M <= size; ++start)
            {
                //-- will remove the constant term
                real_t a0 = 0;
                if(true)
                {
                    for( size_t i=0; i<M; ++i )
                    {
                        assert(start+i<size);
                        a0 += data[ start+i ];
                    }
                }
                a0 /= real_t(M);
                
                //-- load the sample
                for( size_t i=0; i<M; ++i )
                {
                    assert(start+i<size);
                    cplx_t &f = input[i];
                    f.re = (data[ start+i ]-a0) * weight[i];
                    f.im = 0;
                }
                FFT(input,M);
                for(size_t i=0;i<m;++i)
                    p[i] += input[i].mod2();
                ++nsub;
            }
            
            //==================================================================
            // normalize
            //==================================================================
            const real_t nfac = REAL(1.0)/(nsub*sumw2);
            for(size_t i=0;i<m;++i)
                p[i] *= nfac;
        }
        
        template <>
        void PSD<real_t>:: Compute(Window &w, array<real_t> &psd, const array<real_t> &data)
        {
            PSD<real_t>::Compute( w, psd(0), psd.size(), data(0), data.size() );
        }
        
        
    }
	
}
