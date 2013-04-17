#ifndef YOCTO_MATH_FFT_PSD_INCLUDED
#define YOCTO_MATH_FFT_PSD_INCLUDED 1

#include "yocto/math/complex.hpp"
#include "yocto/math/types.hpp"

namespace yocto
{
	namespace math
	{
		
		//! Power Spectral Density
		template <typename T>
		struct PSD
		{
			typedef typename numeric<T>::function Window;
			static  T Square( T )   throw(); //!< Square Function, Test Only!
            static  T Welch( T )    throw(); //!<  1-((x-0.5)/0.5)^2
			static  T Bartlett( T ) throw(); //!<  1-|(x-0.5)/0.5|
			static  T Hann( T )     throw(); //!<  1- cos(2 \pi x )
            static  T Blackman(T)   throw(); //!<  (1-0.16)/2 - 0.5 * cos(2\pi x) + 0.16/2 cos(4\pi x)
            static  T Nutall(T)     throw(); //!< 0.355768 - 0.487396 * cos(2*pi*x) + 0.144232 * cos(4*pi*x) - 0.012604 * cos(6*pi*x)
            
			//! Compute with windowing
			/**
			 \param w       a windowing function
			 \param p       [0..m-1] power spectral density
			 \param m       desired  length, using a M = 2*m sub-sampling
			 \param data    [0..size-1] input signal
			 \param size    data size >= 2*m
			 \warning M=2*m must be a less than size
			 \return M
			 
			 If dt (in s ) is the real space data samping rate, then
			 the p[j] is the spectral density at j / (M*dt) (in Hz).
			 
			 Accordingly, if you want to compute a spectrum
			 with a resolution Freq_min and with frequency lower
			 than Freq_max, you must extract a signal
			 with delta_t <= 1/(2Freq_max) and
			 m >= Freq_max/Freq_min.
			 The total number of points must be >= 2*m
			 */
            static
            void Compute(Window      &w,
                         T           *p,
                         const size_t m,
                         const T     *data,
                         const size_t size,
                         const size_t K);
            
            static
            void Compute(Window &w, array<T> &psd, const array<T> &data, const size_t K);
            
		};
		
	}
}

#endif
