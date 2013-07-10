#include "yocto/utest/run.hpp"
#include "yocto/gfx/surface.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(surface)
{
    const gfx::color_format fmt = gfx::color_format::RGBA32();
    gfx::surface            surf(fmt,512,256);
    
}
YOCTO_UNIT_TEST_DONE()
