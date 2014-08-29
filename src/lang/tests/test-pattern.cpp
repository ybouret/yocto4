#include "yocto/lang/pattern.hpp"
#include "yocto/utest/run.hpp"

#include "yocto/ios/icstream.hpp"


using namespace yocto;

YOCTO_UNIT_TEST_IMPL(pattern)
{
    std::cerr << "sizeof(lang::pattern)=" << sizeof(lang::pattern) << std::endl;
}
YOCTO_UNIT_TEST_DONE()
