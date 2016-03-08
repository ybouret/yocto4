#include "yocto/utest/run.hpp"
#include "yocto/graphics/ops/fft.hpp"
#include "yocto/graphics/image/png.hpp"
#include "yocto/graphics/image/jpeg.hpp"
#include "yocto/graphics/image/tiff.hpp"
#include "yocto/code/utils.hpp"

using namespace yocto;
using namespace graphics;

YOCTO_UNIT_TEST_IMPL(fft)
{
    image &IMG = image::instance();

    IMG.declare( new png_format()  );
    IMG.declare( new jpeg_format() );
    IMG.declare( new tiff_format() );


    //const image::format &PNG = IMG["PNG"];
    const imageIO       &gfx = IMG;
    get_gsz z2gs;

    if(argc>1)
    {
        const string filename = argv[1];

        // load
        std::cerr << "Loading FLOAT" << std::endl;
        pixmapf pf( IMG.loadf(filename, NULL));

        gfx.save("gs.png", pf, NULL);

        const unit_t w = pf.w;
        const unit_t h = pf.h;

        fft work(w,h);

        work.load(pf);
        work.dispatch();
        gfx.save("work.png",work.data,z2gs,NULL);
        work.forward();
        gfx.save("wfwd.png",work.data,z2gs,NULL);
        work.reverse();
        gfx.save("wrev.png",work.data,z2gs,NULL);


    }

}
YOCTO_UNIT_TEST_DONE()


