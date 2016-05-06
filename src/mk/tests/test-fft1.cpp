#include "yocto/utest/run.hpp"
#include "yocto/math/fft/fft1.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/math/types.hpp"
#include "yocto/ios/ocstream.hpp"

#include "yocto/code/xbitrev.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/sys/rt-clock.hpp"

#include <typeinfo>

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


    template <typename T>
    static inline
    void check_v2()
    {
        typedef complex<T> cplx_t;
        uint64_t           v1 = 0;
        uint64_t           v2 = 0;
        uint64_t           mark = 0;
        for( size_t p=0; p <= 12; ++p )
        {
            const size_t   n  = (1<<p);
            std::cerr << "checking v2 for " << n << typeid(T).name() << std::endl;
            vector<T>      data1(n);
            vector<T>      data2(n);
            vector<cplx_t> fft1(n);
            vector<cplx_t> fft2(n);

            vector<cplx_t> c1(n),c2(n);

            T rms1=0, rms2=0;
            size_t count = 0;
            for(size_t iter=0;iter<16;++iter)
            {
                for(size_t i=1;i<=n;++i)
                {
                    data1[i] = c1[i].re = T(0.5)-alea<T>();
                    data2[i] = c2[i].re = T(0.5)-alea<T>();
                }

                mark = rt_clock::ticks();
                FFT(data1(),data2(),fft1(),fft2(),n);
                v2 += rt_clock::ticks() - mark;

                mark = rt_clock::ticks();
                FFT(c1(),n);
                FFT(c2(),n);
                v1 += rt_clock::ticks() - mark;

                for(size_t i=1;i<=n;++i)
                {
                    rms1 += Fabs(c1[i]-fft1[i]);
                    rms2 += Fabs(c2[i]-fft2[i]);
                }

                ++count;
            }
            count *= n;
            rms1 /= count;
            rms2 /= count;
            std::cerr << "\trms@" << n << "=" << rms1 << "," << rms2 << std::endl;
        }

        rt_clock tmx;
        const double ell1 = tmx(v1);
        const double ell2 = tmx(v2);
        std::cerr << "v1=" << v1 << std::endl;
        std::cerr << "v2=" << v2 << std::endl;
        std::cerr << "SPEEDUP=" << ell1/ell2 << std::endl;
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
    
    check_v2<float>();
    check_v2<double>();
    
    perform<float>();
    perform<double>();
}
YOCTO_UNIT_TEST_DONE()
