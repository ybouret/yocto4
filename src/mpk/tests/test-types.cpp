#include "yocto/utest/run.hpp"
#include "yocto/mpk/types.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(types)
{
    mpk::allocator &MPK = mpk::allocator::instance();

    std::cerr << "name=" << MPK.name << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()
