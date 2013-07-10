#include "yocto/utest/run.hpp"
#include "yocto/gfx/pixel-buffer.hpp"

using namespace yocto;

#define __SHOW(VAR) std::cerr << #VAR " = " << px->VAR << std::endl

static inline
void __show_px( const gfx::pixel_buffer &px )
{
    __SHOW(bytes_per_pixel);
    __SHOW(w);
    __SHOW(h);
    __SHOW(stride);
    __SHOW(bytes);
    
    std::cerr << std::endl;
}

YOCTO_UNIT_TEST_IMPL(pixbuf)
{

    gfx::pixel_buffer px1 = new gfx::pixbuf(1,100,20);
    gfx::pixel_buffer px2 = new gfx::pixbuf(2,100,20);
    gfx::pixel_buffer px3 = new gfx::pixbuf(3,100,20);
    gfx::pixel_buffer px4 = new gfx::pixbuf(4,100,20);

    __show_px(px1);
    __show_px(px2);
    __show_px(px3);
    __show_px(px4);

    
}
YOCTO_UNIT_TEST_DONE()
