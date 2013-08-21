#include "yocto/utest/run.hpp"
#include "yocto/gfx/pixel-buffer.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(pixbuf)
{
    
    gfx::pixel_buffer p1 = new gfx::pixbuf(4,100,80);
    {
        gfx::pixel_buffer p2 = p1;
    }
    std::cerr << "#ref=" << p1->refcount() << std::endl;

}
YOCTO_UNIT_TEST_DONE()
