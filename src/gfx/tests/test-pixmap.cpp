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
    std::cerr << "pixmap<char>" << std::endl;
    gfx::pixmap<char>       chpx(250,200);
    std::cerr << "pixmap<double>" << std::endl;
    gfx::pixmap<double>     dpx(100,300);
    std::cerr << "pixmap<rgb_t>" << std::endl;
    gfx::pixmap<gfx::rgb_t> rgbpx(40,50);
    
    std::cerr << "pixmap/filter" << std::endl;
    functor<float,TL1(gfx::rgb_t)> filter( cfunctor(rgb2g) );
    gfx::pixmap<float>             fpx(rgbpx,filter);
    std::cerr << "done" << std::endl;
}
YOCTO_UNIT_TEST_DONE()
