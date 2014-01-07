#include "yocto/utest/run.hpp"
#include "yocto/code/bmove.hpp"
#include "yocto/math/complex.hpp"
#include "yocto/sys/timings.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/hashing/sha1.hpp"
#include "yocto/string/conv.hpp"

#include <cstring>
#include <cstdlib>


using namespace yocto;
using namespace math;


#define Y_BMOVE_TMX(N)  do {                               \
memset(arr,rand(),sizeof(arr));                            \
core::bmove<N>(arr,brr);                                   \
if( H.key<uint64_t>(arr,N)!=H.key<uint64_t>(brr,N) )       \
throw exception("invalid bmove<%u>",N);                    \
YOCTO_TIMINGS(chrono,duration,core::bmove<N>(arr,brr));    \
ios::ocstream fp("mov.dat",true);                          \
fp("%g %g\n", double(N), 1e-6 * chrono.speed );            \
std::cerr << "."; std::cerr.flush();                       \
} while(false)

YOCTO_UNIT_TEST_IMPL(bmove)
{
    
    
    uint8_t arr[64] = { 0 };
    uint8_t brr[64] = { 0 };
    
    double duration = 0.1;
    
    for(unsigned i=0;i<64;++i) brr[i] = uint8_t(i);
    hashing::sha1 H;
    timings chrono;
    
    
    ios::ocstream::overwrite("mov.dat");
    /*         */    Y_BMOVE_TMX(1);  Y_BMOVE_TMX(2);  Y_BMOVE_TMX(3);  Y_BMOVE_TMX(4);  Y_BMOVE_TMX(5);  Y_BMOVE_TMX(6);  Y_BMOVE_TMX(7);  Y_BMOVE_TMX(8);  Y_BMOVE_TMX(9);
    Y_BMOVE_TMX(10); Y_BMOVE_TMX(11); Y_BMOVE_TMX(12); Y_BMOVE_TMX(13); Y_BMOVE_TMX(14); Y_BMOVE_TMX(15); Y_BMOVE_TMX(16); Y_BMOVE_TMX(17); Y_BMOVE_TMX(18); Y_BMOVE_TMX(19);
    Y_BMOVE_TMX(20); Y_BMOVE_TMX(21); Y_BMOVE_TMX(22); Y_BMOVE_TMX(23); Y_BMOVE_TMX(24); Y_BMOVE_TMX(25); Y_BMOVE_TMX(26); Y_BMOVE_TMX(27); Y_BMOVE_TMX(28); Y_BMOVE_TMX(29);
    Y_BMOVE_TMX(30); Y_BMOVE_TMX(31); Y_BMOVE_TMX(32); Y_BMOVE_TMX(33); Y_BMOVE_TMX(34); Y_BMOVE_TMX(35); Y_BMOVE_TMX(36); Y_BMOVE_TMX(37); Y_BMOVE_TMX(38); Y_BMOVE_TMX(39);
    Y_BMOVE_TMX(40); Y_BMOVE_TMX(41); Y_BMOVE_TMX(42); Y_BMOVE_TMX(43); Y_BMOVE_TMX(44); Y_BMOVE_TMX(45); Y_BMOVE_TMX(46); Y_BMOVE_TMX(47); Y_BMOVE_TMX(48); Y_BMOVE_TMX(49);
    Y_BMOVE_TMX(50); Y_BMOVE_TMX(51); Y_BMOVE_TMX(52); Y_BMOVE_TMX(53); Y_BMOVE_TMX(54); Y_BMOVE_TMX(55); Y_BMOVE_TMX(56); Y_BMOVE_TMX(57); Y_BMOVE_TMX(58); Y_BMOVE_TMX(59);
    Y_BMOVE_TMX(60); Y_BMOVE_TMX(61); Y_BMOVE_TMX(62); Y_BMOVE_TMX(63); Y_BMOVE_TMX(64);
    
    std::cerr << std::endl;
    {
        char a=0;
        char b='a';
        bmove(a, b);
        std::cerr << "a=" << a << std::endl;
    }
    
    {
        int16_t a=0;
        int16_t b='a';
        bmove(a, b);
        std::cerr << "a=" << a << std::endl;
    }
    
    {
        uint32_t a=0;
        uint32_t b='a';
        bmove(a, b);
        std::cerr << "a=" << a << std::endl;
    }
    
    {
        uint32_t a=0;
        uint32_t b='a';
        bmove(a, b);
        std::cerr << "a=" << a << std::endl;
    }
    
    {
        uint64_t a=0;
        uint64_t b='a';
        bmove(a, b);
        std::cerr << "a=" << a << std::endl;
    }
    
    
    
    
}
YOCTO_UNIT_TEST_DONE()
