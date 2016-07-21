#include "yocto/gfx/ops/dct.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace gfx;
YOCTO_UNIT_TEST_IMPL(dct)
{

    DCT<8> dct8;
    std::cerr << "dct8=" << dct8 << std::endl;
}
YOCTO_UNIT_TEST_DONE()
