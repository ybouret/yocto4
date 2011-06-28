#ifndef YOCTO_MATH_FFT_PSD_INCLUDED
#define YOCTO_MATH_FFT_PSD_INCLUDED 1

#include "yocto/math/complex.hpp"
#include "yocto/math/types.hpp"

namespace yocto 
{
	namespace math
	{
		enum PSD_Option
		{
			PSD_Regular    = 0x00,
			PSD_Overlap    = 0x01
		};
		
		//! Power Spectral Density
		template <typename T>
		struct PSD
		{
			typedef typename numeric<T>::function Window;
			static  T Square( T );   //!< Square Function, Test Only!
			static  T Bartlett( T ); //!<  1-|(x-0.5)/0.5|
			static  T Hann( T );     //!<  1- cos(2 \pi x )
			static  T Welch( T );    //!<  1-((x-0.5)/0.5)^2
			
			//! truncated gaussian window
			class     Gaussian
			{
			public:
				const T sigma;
				T operator()( T ) const;
				Gaussian( T sig ) throw();
				~Gaussian() throw();
				Gaussian( const Gaussian & ) throw();
				
				YOCTO_DISABLE_ASSIGN(Gaussian);
			};
			
			//! generic blackman window
			class Blackman
			{
			public:
				const T a0, a1, a2;
				T operator()( T ) const;     //!< a0 - a1 * cos( 2 \pi x ) + a2 * cos( 4 \pi x )
				Blackman( T alpha ) throw(); //!< default should be 0.16
				~Blackman() throw();
				Blackman( const Blackman & ) throw();
				
				YOCTO_DISABLE_ASSIGN(Blackman);
			};
			
			//! Compute with windowing
			/**
			 \param w       a windowing function
			 \param p       [0..m-1] power spectral density
			 \param m       desired  length, using a M = 2*m sub-sampling
			 \param data    [0..size-1] input signal
			 \param size    a power of two
			 \param overlap use overlapping of m points
			 \warning M=2*m must be a less than size
			 \return M
			 
			 If dt (in s ) is the real space data samping rate, then
			 the p[j] is the spectral density at j / (M*dt) (in Hz).
			 
			 Accordingly, if you want to compute a spectrum 
			 with a resolution Freq_min and with frequency lower
			 than Freq_max, you must extract a signal
			 with delta_t <= 1/(2Freq_max) and
			 m >= Freq_max/Freq_min.
			 The total number of points must be >= 2*m, so that
			 the data size must be = 2^N * (2*m) (N in [0..]) 
			 with the right sampling rate.
			 */
			static
			size_t Compute(Window       &w,
						   T            *p,
						   const size_t  m,
						   const T      *data, 
						   const size_t  size,
						   const size_t  options);
			
		};
		
	}
}

#endif
