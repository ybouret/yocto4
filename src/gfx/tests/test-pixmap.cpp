#include "yocto/gfx/ops/samples.hpp"
#include "yocto/gfx/rawpix.hpp"
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
    pixmap<YUV>        p12(p3,YUV::fromRGB,p3);
    pixmaps<YUV::type> channels(3,w,h);

    threading::engine server(true);
    xpatches          xps;
    xpatch::create(xps,p12,&server);

    samples<YUV> yuv_samples;
    yuv_samples.split(channels,p12,xps,&server);

    pixmap<RGB>  img(w,h);
    for(unit_t j=0;j<h;++j)
    {
        for(unit_t i=0;i<w;++i)
        {
            const unit_t b = gist::float2byte(channels[0][j][i]);
            img[j][i] = RGB(b,b,b);
        }
    }

    img.save("img.bmp");

}
YOCTO_UNIT_TEST_DONE()
