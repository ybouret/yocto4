#include "yocto/math/fft/fft1.hpp"

namespace yocto
{
	
	namespace math
	{
		
		void FFT( complex<float> *data, const size_t size ) throw()
		{
			assert( data != NULL );
			Fourier<float>::Forward( & data[0].re, size );
		}
		
		void iFFT( complex<float> *data, const size_t size ) throw()
		{
			assert( data != NULL );
			Fourier<float>::Reverse( & data[0].re, size );
		}
		
		void FFT( complex<double> *data, const size_t size ) throw()
		{
			assert( data != NULL );
			Fourier<double>::Forward( & data[0].re, size );
		}
		
		void iFFT( complex<double> *data, const size_t size ) throw()
		{
			assert( data != NULL );
			Fourier<double>::Reverse( & data[0].re, size );
		}
		
        void FFT(const float    *data1,
                 const float    *data2,
                 complex<float> *fft1,
                 complex<float> *fft2,
                 const size_t    size) throw()
        {
            Fourier<float>::Transform(data1, data2, & fft1[0].re, & fft2[0].re, size);
        }
        
        void FFT(const double    *data1,
                 const double    *data2,
                 complex<double> *fft1,
                 complex<double> *fft2,
                 const size_t    size) throw()
        {
            Fourier<double>::Transform(data1, data2, & fft1[0].re, & fft2[0].re, size);
        }

	}

}
