#include "yocto/code/utils.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(utils)
{
    for( size_t i=0; i <= 1000; i+=10)
    {
        std::cerr << i << " => " << next_power_of_two<size_t>(i) << std::endl;
    }
}
YOCTO_UNIT_TEST_DONE()
