#include "yocto/utest/run.hpp"

#include "yocto/graphics/image/png.hpp"
#include "yocto/graphics/image/jpeg.hpp"
#include "yocto/graphics/image/tiff.hpp"
#include "yocto/graphics/ops/histogram.hpp"
#include "yocto/graphics/ops/blobs.hpp"
#include "yocto/graphics/ops/gradient.hpp"
#include "yocto/graphics/ops/blur.hpp"
#include "yocto/graphics/ops/particles.hpp"
#include "yocto/graphics/ops/differential.hpp"

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
        const size_t links = 8;
        tagmap tmap(w,h);
        tmap.build(fg,links);

        get_named_color<size_t> tag2color;
        PNG.save("image_tag.png",tmap,tag2color,NULL);
        
        particles pa;
        pa.load(tmap);
        std::cerr << "#particles=" << pa.size() << std::endl;

        pixmap3 part(w,h);

        if(pa.size())
        {
            particle &big = *pa[1];
            std::cerr << "big.size=" << big.size << std::endl;
            std::cerr << "-- Pick up big particle" << std::endl;
            big.transfer(part,fg);
            PNG.save("image_big.png",part, NULL);

            std::cerr << "-- Split particle border" << std::endl;
            big.split_using(tmap);
            std::cerr << "big.border=" << big.border.size << std::endl;
            part.ldz();
            big.transfer_with_contour(part,fg,named_color::get("yellow"));
            PNG.save("image_big2.png",part, NULL);

            std::cerr << "-- Dilate particle..." << std::endl;
            big.dilate_with(tmap);

            std::cerr << "-- Split again!" << std::endl;
            big.split_using(tmap);
            part.ldz();

            std::cerr << "-- Transfer new border..." << std::endl;
            big.transfer_with_contour(part,fg,named_color::get("red"));
            PNG.save("image_big3.png",part, NULL);
        }

        // 
        tmap.build(fg,links);
        pa.load(tmap);
        std::cerr << "#pa_org=" << pa.size() << std::endl;
        {
            part.ldz();
            for(size_t i=1;i<=pa.size();++i)
            {
                pa[i]->split_using(tmap);
                pa[i]->transfer_contour(part, named_color::fetch(i));
            }
            PNG.save("image_pa_contours.png",part,NULL);

        }

        {
            std::cerr << "-- Fusion" << std::endl;
            pa.fusion(tmap);
            PNG.save("image_tag2.png",tmap,tag2color,NULL);
            part.ldz();
            for(size_t i=1;i<=pa.size();++i)
            {
                pa[i]->transfer_contour(part, named_color::fetch(i));
            }
            PNG.save("image_pa_contours2.png",part,NULL);
            std::cerr << "#pa_new=" << pa.size() << std::endl;
        }



    }

    std::cerr << "sizeof(particle) =" << sizeof(particle)  << std::endl;
    std::cerr << "sizeof(particles)=" << sizeof(particles) << std::endl;
}
YOCTO_UNIT_TEST_DONE()
