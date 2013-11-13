#include "yocto/utest/run.hpp"
#include "yocto/code/bzset.hpp"
#include "yocto/sys/timings.hpp"
#include "yocto/ios/ocstream.hpp"
#include <cstring>

using namespace yocto;

#define Y_BZSET_TMX(N) \
do{ \
for(size_t i=0;i<sizeof(arr)/sizeof(arr[0]);++i) arr[i] = uint8_t(i+1);\
YOCTO_TIMINGS(chrono,duration,core::bzfill<N>(arr));                   \
for(unsigned i=0;i<N;++i) if(arr[i]!=0)                                \
throw exception("error for bzset<%u>@%u", N, i);                       \
speed = chrono.speed;                                                  \
for(size_t i=0;i<sizeof(arr)/sizeof(arr[0]);++i) arr[i] = uint8_t(i+1);\
YOCTO_TIMINGS(chrono,duration,memset(arr,0,N));                        \
speed2 = chrono.speed;                                                 \
ios::ocstream fp("ldz.dat",true);                                      \
fp("%u %.5g %.5g\n", unsigned(N), 1e-6*speed, 1e-6*speed2);            \
std::cerr << '.';                                                      \
std::cerr.flush();                                                     \
} while(false)

#include "yocto/string/conv.hpp"

YOCTO_UNIT_TEST_IMPL(bzset)
{
    uint8_t arr[64] = {0};
    timings chrono;
    
    ios::ocstream::overwrite("ldz.dat");
    double speed    = 0;
	double duration = 0.08;
    double speed2   = 0;
    
    if(argc>1)
    {
        duration = strconv::to<double>(argv[1],"duration");
    }
    
    /*         */    Y_BZSET_TMX(1);  Y_BZSET_TMX(2);  Y_BZSET_TMX(3);  Y_BZSET_TMX(4);  Y_BZSET_TMX(5);  Y_BZSET_TMX(6);  Y_BZSET_TMX(7);  Y_BZSET_TMX(8);  Y_BZSET_TMX(9);
    Y_BZSET_TMX(10); Y_BZSET_TMX(11); Y_BZSET_TMX(12); Y_BZSET_TMX(13); Y_BZSET_TMX(14); Y_BZSET_TMX(15); Y_BZSET_TMX(16); Y_BZSET_TMX(17); Y_BZSET_TMX(18); Y_BZSET_TMX(19);
    Y_BZSET_TMX(20); Y_BZSET_TMX(21); Y_BZSET_TMX(22); Y_BZSET_TMX(23); Y_BZSET_TMX(24); Y_BZSET_TMX(25); Y_BZSET_TMX(26); Y_BZSET_TMX(27); Y_BZSET_TMX(28); Y_BZSET_TMX(29);
    Y_BZSET_TMX(30); Y_BZSET_TMX(31); Y_BZSET_TMX(32); Y_BZSET_TMX(33); Y_BZSET_TMX(34); Y_BZSET_TMX(35); Y_BZSET_TMX(36); Y_BZSET_TMX(37); Y_BZSET_TMX(38); Y_BZSET_TMX(39);
    Y_BZSET_TMX(40); Y_BZSET_TMX(41); Y_BZSET_TMX(42); Y_BZSET_TMX(43); Y_BZSET_TMX(44); Y_BZSET_TMX(45); Y_BZSET_TMX(46); Y_BZSET_TMX(47); Y_BZSET_TMX(48); Y_BZSET_TMX(49);
    Y_BZSET_TMX(50); Y_BZSET_TMX(51); Y_BZSET_TMX(52); Y_BZSET_TMX(53); Y_BZSET_TMX(54); Y_BZSET_TMX(55); Y_BZSET_TMX(56); Y_BZSET_TMX(57); Y_BZSET_TMX(58); Y_BZSET_TMX(59);
    Y_BZSET_TMX(60); Y_BZSET_TMX(61); Y_BZSET_TMX(62); Y_BZSET_TMX(63); Y_BZSET_TMX(64);
    
    std::cerr << std::endl;
}
YOCTO_UNIT_TEST_DONE()
