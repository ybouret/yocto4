#include "yocto/utest/run.hpp"
#include "yocto/gfx/pixmap.hpp"
#include "yocto/gfx/rgb.hpp"

using namespace yocto;

float rgb2g(const gfx::rgb_t &c)
{
    return gfx::conv::greyscale_f(c.r, c.g, c.b);
}

YOCTO_UNIT_TEST_IMPL(pixmap)
{
    
    gfx::pixmap<char>       chpx(250,200);
    gfx::pixmap<double>     dpx(100,300);
    gfx::pixmap<gfx::rgb_t> rgbpx(40,50);
    functor<float,TL1(gfx::rgb_t)> filter( cfunctor(rgb2g) );
    gfx::pixmap<float>      fpx(rgbpx,filter);
}
YOCTO_UNIT_TEST_DONE()
