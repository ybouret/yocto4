#include "yocto/utest/run.hpp"
#include "yocto/mpk/natural.hpp"

using namespace yocto;
using namespace mpk;

YOCTO_UNIT_TEST_IMPL(mpn)
{
    natural_word w;
    std::cerr << "w=" << w << std::endl;
    w = 1;
    std::cerr << "w=" << w << std::endl;
    w = 0xABC;
    std::cerr << "w=" << w << std::endl;
}
YOCTO_UNIT_TEST_DONE()
