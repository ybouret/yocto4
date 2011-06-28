#include "yocto/math/fft/fft1.hpp"

namespace yocto
{
	
	namespace math
	{
		
		void FFT( complex<float> *data, size_t size ) throw()
		{
			assert( data != NULL );
			Fourier<float>::Forward( & data[0].re, size );
		}
		
		void iFFT( complex<float> *data, size_t size ) throw()
		{
			assert( data != NULL );
			Fourier<float>::Reverse( & data[0].re, size );
		}
		
		void FFT( complex<double> *data, size_t size ) throw()
		{
			assert( data != NULL );
			Fourier<double>::Forward( & data[0].re, size );
		}
		
		void iFFT( complex<double> *data, size_t size ) throw()
		{
			assert( data != NULL );
			Fourier<double>::Reverse( & data[0].re, size );
		}
		
		
		
	}
	
}
