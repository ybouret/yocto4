#include "yocto/code/utils.hpp"
#include "yocto/code/bin2name.hpp"
#include "yocto/utest/run.hpp"

#include <cstring>

using namespace yocto;

#define SHOW_FOR(TYPE) std::cerr << "sizeof(bin2id<" << #TYPE << ">)=" <<  sizeof(bin2id<TYPE>) << std::endl

YOCTO_UNIT_TEST_IMPL(utils)
{


    for( size_t i=1; i <= 1000; i+=10)
    {
        std::cerr << i << " => " << next_power_of_two<size_t>(i) << std::endl;
    }

    SHOW_FOR(int);
    SHOW_FOR(int8_t);
    SHOW_FOR(int16_t);
    SHOW_FOR(uint32_t);
    SHOW_FOR(int64_t);
    
    for(int i=0;i<10;++i)
    {
        bin2id<int> b(&i);
        std::cerr << "b_int(" << i << ")=" << b.value << std::endl;
    }
}
YOCTO_UNIT_TEST_DONE()
