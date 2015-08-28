#include "yocto/utest/run.hpp"
#include "yocto/gfx/image/jpeg.hpp"
#include "yocto/gfx/image/png.hpp"
#include "yocto/gfx/image/tiff.hpp"
#include "yocto/gfx/ops/channels.hpp"
#include "yocto/gfx/rawpix.hpp"

using namespace yocto;
using namespace gfx;


YOCTO_UNIT_TEST_IMPL(ops)
{
    image &IMG = image::instance();

    IMG.declare( new png_format()  );
    IMG.declare( new jpeg_format() );
    IMG.declare( new tiff_format() );

    const image::format &PNG = IMG["PNG"];

    //threading::SIMD simd;

    threading::server psrv;
    if(argc>1)
    {
        const string         filename = argv[1];

        // load image
        bitmap::pointer bmp(IMG.load(filename,3, put_rgba::to_rgb,NULL,NULL));

        // make a pixmap, RGB
        pixmap3         pxm(bmp,NULL);
        PNG.save("image3.png",pxm, get_rgba::from_rgb,NULL, NULL);

        // make a hard copy
        pixmap3         pxm2(pxm,NULL);
        const unit_t w = pxm.w;
        const unit_t h = pxm.h;

        pixmaps<uint8_t> ch (3,w,h);

        // prepare splitting
        channels<>::iopatches chn_ops;
        chn_ops.setup_for(psrv.size,pxm);

        chn_ops.split(ch,pxm,0);
        

    }
    
    
}
YOCTO_UNIT_TEST_DONE()
