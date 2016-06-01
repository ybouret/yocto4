#include "yocto/utest/run.hpp"

#include "yocto/graphics/image/png.hpp"
#include "yocto/graphics/image/jpeg.hpp"
#include "yocto/graphics/image/tiff.hpp"
#include "yocto/graphics/ops/histogram.hpp"
#include "yocto/graphics/ops/blobs.hpp"

using namespace yocto;
using namespace graphics;

YOCTO_UNIT_TEST_IMPL(pa)
{
    image &IMG = image::instance();

    IMG.declare( new png_format()  );
    IMG.declare( new jpeg_format() );
    IMG.declare( new tiff_format() );

    const image::format &PNG = IMG["PNG"];

    threading::engine server(4,0,true);

    if(argc>1)
    {
        const string filename = argv[1];
        std::cerr << "-- Loading RGB" << std::endl;
        pixmap3 bmp( IMG.load3(filename, NULL)); PNG.save("image.png",    bmp, NULL);
        pixmapf pgs(bmp,to_float<RGB>,bmp);      PNG.save("image_gs.png", pgs, NULL);
        const unit_t w = pgs.w;
        const unit_t h = pgs.h;

        xpatches xps;
        xpatch::create(xps, bmp, &server);

        std::cerr << "-- Creating Histogram" << std::endl;
        histogram H;
        H.update(pgs,xps, &server);
        pixmapf fg(w,h);

        std::cerr << "-- Thresholding" << std::endl;
        threshold::apply(fg, H.threshold(), pgs, threshold::keep_foreground);
        PNG.save("image_fg.png",fg,NULL);

        std::cerr << "-- Making blobs..." << std::endl;

        blobs B(w,h);
        B.build(fg);
        get_named_color<size_t> blobColors;
        std::cerr << "#blobs=" << B.current << std::endl;
        PNG.save("image_fg_blobs.png", B, blobColors, NULL);


        pixmap3 tgt(w,h);
        if(B.content.size()>0)
        {
            const blob &big = *B.content[1];
            big.transfer(tgt,bmp);
        }
        PNG.save("image_fg_main.png",tgt,NULL);
        
    }
    
}
YOCTO_UNIT_TEST_DONE()