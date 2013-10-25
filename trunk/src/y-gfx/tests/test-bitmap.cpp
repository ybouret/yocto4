#include "yocto/utest/run.hpp"
#include "yocto/gfx/bitmap.hpp"


using namespace yocto;


YOCTO_UNIT_TEST_IMPL(bitmap)
{
    gfx::bitmap::pointer bmp( gfx::bitmap::create(4,200,100) );
    
}
YOCTO_UNIT_TEST_DONE()
