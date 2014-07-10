#include "yocto/utest/run.hpp"
#include "yocto/math/q64.hpp"

using namespace yocto;
using math::q64_t;

YOCTO_UNIT_TEST_IMPL(q64)
{
    q64_t q;
    std::cerr << "q=" << q << std::endl;
}
YOCTO_UNIT_TEST_DONE()

