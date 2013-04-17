#ifndef YOCTO_MATH_FFT1_INCLUDED
#define YOCTO_MATH_FFT1_INCLUDED 1

#include "yocto/math/complex.hpp"


namespace yocto
{
	
	namespace math
	{
		
		//! normalized fast fourier transform
		template <typename T>
		struct Fourier
		{
			//! FFT(data,nn)
			/**
			 data[0] = re, data[1]=im,....
			 */
			static void Transform( T *data, const size_t size, const int isign) throw();
			
			//! Forward Transform
			static inline void Forward( T *data, const size_t size ) throw()
			{
				Transform(data,size,1);
			}
			
			//! Reverse Transform
			static inline void Reverse( T *data, const size_t size ) throw()
			{
				Transform(data,size,-1);
			}
			
			
		};
		
		
		void FFT(  complex<float>  *data, size_t size ) throw(); //!< Fourier Transform of data[0..size-1]
		void iFFT( complex<float>  *data, size_t size ) throw(); //!< Inverse Fourier Transform of data[0..size-1]
		void FFT(  complex<double> *data, size_t size ) throw(); //!< Fourier Transform of data[0..size-1]
		void iFFT( complex<double> *data, size_t size ) throw(); //!< Inverse Fourier Transform of data[0..size-1]
			
	}
	
}

#endif
