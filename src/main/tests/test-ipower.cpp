#include "yocto/utest/run.hpp"
#include "yocto/code/ipower.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(ipower)
{
    for(size_t i=0;i<=8;++i)
    {
        std::cerr << "5^" << i << " = " << ipower(5,i) << std::endl;
    }
}
YOCTO_UNIT_TEST_DONE()
