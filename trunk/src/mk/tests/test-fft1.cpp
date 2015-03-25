#include "yocto/utest/run.hpp"
#include "yocto/math/fft/fft1.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/math/types.hpp"
#include "yocto/ios/ocstream.hpp"

#include "yocto/code/xbitrev.hpp"

using namespace yocto;
using namespace math;


namespace {
    
    template <typename T>
    static inline
    void perform()
    {
        typedef complex<T> cplx_t;
        
        for( size_t p=0; p <= 12; ++p )
        {
            const size_t   n  = (1<<p);
            vector<cplx_t> data( n, numeric<cplx_t>::zero );
            for( size_t i=1; i <= n; ++i )
            {
                data[i].re = T( (i-1) * (n-i) );
            }
            vector<cplx_t> raw( data );
            vector<T>      spec( data.size(), numeric<T>::zero );

            FFT( data(0), data.size() );
            for( size_t i=1; i <= n; ++i )
            {
                spec[i] = Fabs( data[i] );
            }
            iFFT( data(0), data.size() );
            
            
#if defined(FFT_SAVE) && FFT_SAVE == 1
            const string filename = string("fft") + string( char( 'A' + p ) ) + "-" + typeid(T).name() + ".dat";
            ios::ocstream fp( filename );
#endif
            
            T d = 0.0;
            for( size_t i=1; i <= n; ++i )
            {
#if defined(FFT_SAVE) && FFT_SAVE == 1
                fp("%u %g %g %g\n", i, raw[i].re, spec[i], data[i].re );
#endif
                const T tmp = Fabs(raw[i] - data[i]);
                d += tmp * tmp;
            }
            d /= n;
            d = Sqrt( d );
            std::cerr << "RMS@" << n << "=" << d << std::endl;
            
        }
        
    }
    
}

#define SHOW(TOTO) std::cerr << "sizeof(" #TOTO ") = " << sizeof(TOTO) << std::endl

YOCTO_UNIT_TEST_IMPL(fft1)
{
    SHOW(xbitrev::indx0004);
    SHOW(xbitrev::indx0008);
    SHOW(xbitrev::indx0016);
    SHOW(xbitrev::indx0032);
    SHOW(xbitrev::indx0064);
    SHOW(xbitrev::indx0128);
    SHOW(xbitrev::indx0256);
    SHOW(xbitrev::indx0512);
    SHOW(xbitrev::indx1024);
    SHOW(xbitrev::indx2048);
    SHOW(xbitrev::indx4096);
    SHOW(xbitrev::indx8192);


	perform<float>();
	perform<double>();
}
YOCTO_UNIT_TEST_DONE()
