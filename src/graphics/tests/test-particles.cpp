#include "yocto/utest/run.hpp"

#include "yocto/graphics/image/png.hpp"
#include "yocto/graphics/image/jpeg.hpp"
#include "yocto/graphics/image/tiff.hpp"
#include "yocto/graphics/ops/histogram.hpp"
#include "yocto/graphics/ops/blobs.hpp"
#include "yocto/graphics/ops/gradient.hpp"
#include "yocto/graphics/ops/blur.hpp"
#include "yocto/graphics/ops/particles.hpp"

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
        const unit_t w = bmp.w;
        const unit_t h = bmp.h;

        std::cerr << "-- Preparing Patches" << std::endl;
        xpatches xps;
        xpatch::create(xps, bmp, &server);


        std::cerr << "-- Looking Up Foreground..." << std::endl;
        pixmap3 fg(w,h);
        separate(threshold::keep_foreground,fg,bmp, xps, &server);
        PNG.save("image_fg.png",fg, NULL);

        std::cerr << "-- Building Initial Tag Map" << std::endl;
        tagmap tmap(w,h);
        tmap.build(fg,8);

        get_named_color<size_t> tag2color;
        PNG.save("image_tag.png",tmap,tag2color,NULL);
        
        particles pa;
        pa.load(tmap);
        std::cerr << "#particles=" << pa.size() << std::endl;
        for(size_t i=1;i<=pa.size();++i)
        {
            std::cerr << "size#" << i << "=" << pa[i]->size << std::endl;
        }

    }
    
}
YOCTO_UNIT_TEST_DONE()
