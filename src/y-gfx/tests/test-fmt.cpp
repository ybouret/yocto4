#include "yocto/utest/run.hpp"
#include "yocto/gfx/color-format.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(fmt)
{
    {
        gfx::color_format fmt(4,0xff000000,0x00ff0000,0x0000ff00,0x000000ff);
    }
    
    {
        gfx::color_format fmt(4,0x0f000000,0x00ff0000,0x0000ff00,0x000000ff);
    }
}
YOCTO_UNIT_TEST_DONE()

