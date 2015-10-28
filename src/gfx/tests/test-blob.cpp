#include "yocto/utest/run.hpp"
#include "yocto/gfx/ops/histogram.hpp"
#include "yocto/gfx/image.hpp"

#include "yocto/gfx/image/jpeg.hpp"
#include "yocto/gfx/image/png.hpp"
#include "yocto/gfx/image/tiff.hpp"

#include "yocto/gfx/rawpix.hpp"

using namespace yocto;
using namespace gfx;

YOCTO_UNIT_TEST_IMPL(blob)
{
    image &IMG = image::instance();

    IMG.declare( new png_format()  );
    IMG.declare( new jpeg_format() );
    IMG.declare( new tiff_format() );

    const image::format &PNG = IMG["PNG"];

    if(argc>1)
    {
        const string imgname = argv[1];
        // load a pixmap
        bitmap::pointer bmp(IMG.load(imgname,4, put_rgba::to_gsf,NULL,NULL));
        pixmapf         pxm(bmp,NULL);

        PNG.save("image_bw.png",pxm, get_rgba::from_gsf ,NULL, NULL);




    }

}
YOCTO_UNIT_TEST_DONE()
