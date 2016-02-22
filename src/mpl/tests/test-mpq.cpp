#include "yocto/mpl/rational.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(mpq)
{
    mpq r = 2;
    std::cerr << "r=" << r << std::endl;
}
YOCTO_UNIT_TEST_DONE()
