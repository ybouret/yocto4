#include "yocto/utest/run.hpp"
#include "yocto/code/bswap.hpp"
#include "yocto/math/complex.hpp"
#include "yocto/sys/timings.hpp"
#include "yocto/hashing/sha1.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/string/conv.hpp"

using namespace yocto;
using namespace math;

#include <cstdlib>
#include <cstring>

#define Y_BSWAP_TMX(N)  do {                            \
memset(arr,rand(),sizeof(arr));                         \
const uint64_t akey = H.key<uint64_t>(arr,N) ;          \
const uint64_t bkey = H.key<uint64_t>(brr,N) ;          \
core::bswap<N>(arr,brr);                                \
if( H.key<uint64_t>(arr,N)!=bkey )                      \
throw exception("invalid bswap<%u> level 1",N);         \
if( H.key<uint64_t>(brr,N)!=akey)                       \
throw exception("invalid bswap<%u> level 2",N);         \
YOCTO_TIMINGS(chrono,duration,core::bswap<N>(arr,brr)); \
ios::ocstream fp("swp.dat",true);                       \
fp("%g %g\n", double(N), chrono.speed * 1e-6 );         \
std::cerr << "."; std::cerr.flush();                    \
} while(false)


YOCTO_UNIT_TEST_IMPL(bswap)
{
    uint8_t arr[64] = { 0 };
    uint8_t brr[64] = { 0 };
    
    for(unsigned i=0;i<64;++i) brr[i] = uint8_t(i);
    hashing::sha1 H;
    
    timings chrono;
    
    double duration = 0.1;
    if( argc > 1 )
        duration = strconv::to<double>(argv[1],"duration");
    
    ios::ocstream::overwrite("swp.dat");
    
    
    /*         */    Y_BSWAP_TMX(1);  Y_BSWAP_TMX(2);  Y_BSWAP_TMX(3);  Y_BSWAP_TMX(4);  Y_BSWAP_TMX(5);  Y_BSWAP_TMX(6);  Y_BSWAP_TMX(7);  Y_BSWAP_TMX(8);  Y_BSWAP_TMX(9);
    Y_BSWAP_TMX(10); Y_BSWAP_TMX(11); Y_BSWAP_TMX(12); Y_BSWAP_TMX(13); Y_BSWAP_TMX(14); Y_BSWAP_TMX(15); Y_BSWAP_TMX(16); Y_BSWAP_TMX(17); Y_BSWAP_TMX(18); Y_BSWAP_TMX(19);
    Y_BSWAP_TMX(20); Y_BSWAP_TMX(21); Y_BSWAP_TMX(22); Y_BSWAP_TMX(23); Y_BSWAP_TMX(24); Y_BSWAP_TMX(25); Y_BSWAP_TMX(26); Y_BSWAP_TMX(27); Y_BSWAP_TMX(28); Y_BSWAP_TMX(29);
    Y_BSWAP_TMX(30); Y_BSWAP_TMX(31); Y_BSWAP_TMX(32); Y_BSWAP_TMX(33); Y_BSWAP_TMX(34); Y_BSWAP_TMX(35); Y_BSWAP_TMX(36); Y_BSWAP_TMX(37); Y_BSWAP_TMX(38); Y_BSWAP_TMX(39);
    Y_BSWAP_TMX(40); Y_BSWAP_TMX(41); Y_BSWAP_TMX(42); Y_BSWAP_TMX(43); Y_BSWAP_TMX(44); Y_BSWAP_TMX(45); Y_BSWAP_TMX(46); Y_BSWAP_TMX(47); Y_BSWAP_TMX(48); Y_BSWAP_TMX(49);
    Y_BSWAP_TMX(50); Y_BSWAP_TMX(51); Y_BSWAP_TMX(52); Y_BSWAP_TMX(53); Y_BSWAP_TMX(54); Y_BSWAP_TMX(55); Y_BSWAP_TMX(56); Y_BSWAP_TMX(57); Y_BSWAP_TMX(58); Y_BSWAP_TMX(59);
    Y_BSWAP_TMX(60); Y_BSWAP_TMX(61); Y_BSWAP_TMX(62); Y_BSWAP_TMX(63); Y_BSWAP_TMX(64);

    std::cerr << std::endl;
    
    {
        char a = 'A';
        char b = 'B';
        bswap(a,b);
        std::cerr << a << ' ' << b << std::endl;
    }
    
    {
        short a = 0;
        short b = 1;
        bswap(a,b);
        std::cerr << a << ' ' << b << std::endl;
    }
    
    {
        int32_t a = 0;
        int32_t b = 1;
        bswap(a,b);
        std::cerr << a << ' ' << b << std::endl;
    }
    
    {
        float a = 0;
        float b = 1;
        bswap(a,b);
        std::cerr << a << ' ' << b << std::endl;
    }
    
    
    {
        double a = 0;
        double b = 1;
        bswap(a,b);
        std::cerr << a << ' ' << b << std::endl;
    }
    
    {
        complex<float> a(1,2);
        complex<float> b(3,4);
        bswap(a,b);
        std::cerr << a << ' ' << b << std::endl;
    }
    
    {
        complex<double> a(1,2);
        complex<double> b(3,4);
        bswap(a,b);
        std::cerr << a << ' ' << b << std::endl;
    }
    
}
YOCTO_UNIT_TEST_DONE()
