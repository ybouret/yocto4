#include "yocto/graphix/image/png.hpp"
#include "yocto/graphix/image/jpeg.hpp"
#include "yocto/graphix/image/tiff.hpp"
#include "yocto/graphix/ops/hist.hpp"

#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace graphix;

YOCTO_UNIT_TEST_IMPL(ops)
{
    image &IMG = image::instance();

    IMG.declare( new png_format()  );
    IMG.declare( new jpeg_format() );
    IMG.declare( new tiff_format() );

    if(argc>1)
    {
        const string filename = argv[1];
        pixmap4      pxm( IMG.load4(filename, NULL));

        histogram H;
        H.update(pxm);
        H.save("hist.dat");
    }
}
YOCTO_UNIT_TEST_DONE()
