
#include "yocto/utest/run.hpp"
#include "yocto/graphics/ops/gradient.hpp"

#include "yocto/graphics/image/png.hpp"
#include "yocto/graphics/image/jpeg.hpp"
#include "yocto/graphics/image/tiff.hpp"

using namespace yocto;
using namespace graphics;

YOCTO_UNIT_TEST_IMPL(ops2)
{
    image &IMG = image::instance();

    IMG.declare( new png_format()  );
    IMG.declare( new jpeg_format() );
    IMG.declare( new tiff_format() );

    threading::engine server(true);

    if(argc>1)
    {
        const string filename = argv[1];
        pixmap4      pxm( IMG.load4(filename, NULL));
        const unit_t w = pxm.w;
        const unit_t h = pxm.h;
        xpatches xps;
        xpatch::create(xps, pxm, &server);

        std::cerr << "w=" << w << std::endl;
        std::cerr << "h=" << h << std::endl;
        std::cerr << "#patches=" << xps.size() << std::endl;

        pixmap<float> g(w,h);

        gradient G;
        G.start(g,pxm, xps, NULL);

        server.flush();
        
    }

}
YOCTO_UNIT_TEST_DONE()
