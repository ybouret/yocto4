#include "yocto/utest/run.hpp"
#include "yocto/graphics/ops/gradient.hpp"
#include "yocto/graphics/ops/samples.hpp"
#include "yocto/graphics/ops/histogram.hpp"
#include "yocto/graphics/ops/blobs.hpp"
#include "yocto/graphics/ops/stencil.hpp"
#include "yocto/graphics/ops/fft.hpp"
#include "yocto/graphics/ops/blur.hpp"

#include "yocto/graphics/image/png.hpp"
#include "yocto/graphics/image/jpeg.hpp"
#include "yocto/graphics/image/tiff.hpp"

#include "yocto/sys/timings.hpp"

using namespace yocto;
using namespace graphics;

YOCTO_UNIT_TEST_IMPL(blobs)
{
    image &IMG = image::instance();

    IMG.declare( new png_format()  );
    IMG.declare( new jpeg_format() );
    IMG.declare( new tiff_format() );

    const image::format &PNG = IMG["PNG"];


    threading::engine       server(true);
    get_named_color<size_t> blobColors;

    if(argc>1)
    {
        const string filename = argv[1];
        pixmap3      img( IMG.load3(filename,NULL) );
        PNG.save("image.png", img, NULL);

        xpatches xps;
        xpatch::create(xps, img, &server);


        const unit_t w = img.w;
        const unit_t h = img.h;

        pixmap3     fg(w,h);
        separate(threshold::keep_foreground, fg, img, xps, &server);

        blobs Blobs(w,h);
        Blobs.build(fg,8);
        PNG.save("blobs.png", Blobs, blobColors, NULL);

        if(Blobs.content.size()>0)
        {
            blob    &b1 = *Blobs.content[1];
            pixmap3 I1(w,h);
            b1.transfer(I1,fg);

        }


    }

}
YOCTO_UNIT_TEST_DONE()
