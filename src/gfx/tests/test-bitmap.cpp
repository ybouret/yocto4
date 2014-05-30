#include "yocto/utest/run.hpp"
#include "yocto/gfx/bitmap.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(bitmap)
{
    gfx::bitmap b1(1,100,50);
    gfx::bitmap::pointer pb2a( new gfx::bitmap(2,200,60));
    gfx::bitmap::pointer pb2b( pb2a );
    const gfx::rectangle rect( 10, 20, 40, 20 );
    gfx::bitmap          sb2a( pb2a, 0);
    gfx::bitmap          sb2b( pb2b, &rect);
    gfx::bitmap          b2c( sb2b, 0);
    
}
YOCTO_UNIT_TEST_DONE()
