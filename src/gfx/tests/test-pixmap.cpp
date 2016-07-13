#include "yocto/gfx/pixmaps.hpp"
#include "yocto/gfx/color/yuv.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/code/ipower.hpp"
#include "yocto/string.hpp"
#include <cmath>

using namespace yocto;
using namespace gfx;


YOCTO_UNIT_TEST_IMPL(pixmap)
{

    const unit_t w = 200;
    const unit_t h = 100;

    pixmap<RGB>     p3(w,h);
    pixmap<uint8_t> p1(w,h);
    pixmap<float>   pf(w,h);
    pixmap<RGBA>    p4(w,h);
    pixmap<cplx_t>  pz(w,h);

    
    p3.ld( RGB(255,0,0) );
    pixmap<YUV> p12(p3,YUV::fromRGB,p3);
    pixmaps<YUV::type> channels(w,h,3);



}
YOCTO_UNIT_TEST_DONE()
