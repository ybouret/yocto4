#include "yocto/utest/run.hpp"
#include "yocto/gfx/pixmap.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(pixmap)
{
    
    yocto::gfx::pixmap<char> chpx(250,200);
    
}
YOCTO_UNIT_TEST_DONE()
