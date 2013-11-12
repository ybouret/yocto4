#include "yocto/utest/run.hpp"
#include "yocto/code/bzset.hpp"
#include "yocto/sys/wtime.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;

#define ITER_MAX (32*1024*1024)

#define Y_BZSET_TMX(N) do { \
for(size_t i=0;i<sizeof(arr)/sizeof(arr[0]);++i) arr[i] = uint8_t(i+1); \
const double ini  = chrono.query();                  \
const size_t iter = ITER_MAX/N;                      \
for(size_t i=iter;i>0;--i) { core::bzset<N>(arr); }  \
const double tmx = chrono.query() - ini;             \
const double spd = (iter/tmx);                       \
ios::ocstream fp("ldz.dat",true);                    \
fp("%u %.5g\n", unsigned(N), 1e-6*spd);              \
std::cerr << '.';                                    \
std::cerr.flush();                                   \
for(unsigned i=0;i<N;++i) if(0!=arr[i]) throw exception("invalid bzset<%u>@%u\n", unsigned(N), i);\
} while(false)

YOCTO_UNIT_TEST_IMPL(bzset)
{
    uint8_t arr[64] = {0};
    wtime chrono;
    chrono.start();
    
    ios::ocstream::overwrite("ldz.dat");
    
    
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
