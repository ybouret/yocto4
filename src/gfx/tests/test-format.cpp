#include "yocto/utest/run.hpp"
#include "yocto/gfx/format.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(format)
{
    gfx::format fmt(0x00ff0000,
                    0x0000ff00,
                    0x000000ff,
                    0xff000000);
    
}
YOCTO_UNIT_TEST_DONE()
