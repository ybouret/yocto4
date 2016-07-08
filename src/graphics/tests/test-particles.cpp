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
#include "yocto/graphics/ops/filter.hpp"

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
        std::cerr << "-- Loading image" << std::endl;
        pixmapf bmp( IMG.loadf(filename,NULL) );
        PNG.save("image_gs.png",bmp, NULL);

        pixmap3 img( IMG.load3(filename,NULL) );


        const unit_t w = bmp.w;
        const unit_t h = bmp.h;

        std::cerr << "-- Preparing Patches" << std::endl;
        xpatches xps;
        xpatch::create(xps, bmp, &server);

        pixmapf grd(w,h);
        {
            differential diff(w,h);
            std::cerr << "-- Compute Gradient" << std::endl;
            diff.compute(grd, bmp, use_gradient, xps, &server);
            PNG.save("image_grd.png",grd, NULL);
        }

        pixmapf flt(w,h);
        {
            filter F;
            F.apply(flt,grd, filter_median, xps, &server);
            PNG.save("image_flt.png",flt, NULL);
        }


        std::cerr << "-- Looking Up Foreground..." << std::endl;
        pixmapf fg(w,h);
        separate(threshold::keep_foreground,fg,flt,xps,&server);
        PNG.save("image_fg.png",fg, NULL);

        std::cerr << "-- Building Initial Tag Map" << std::endl;
        tagmap tmap(w,h);
        tmap.build(fg,8);

        get_named_color<size_t> tag2color;
        PNG.save("image_tag.png",tmap,tag2color,NULL);

        std::cerr << "-- Create initial particles" << std::endl;
        particles pa;
        pa.load(tmap);
        std::cerr << "#particles=" << pa.size() << std::endl;

        std::cerr << "-- Finding Borders..." << std::endl;
        pa.split_all_using(tmap);

        pixmap3 part(w,h);

        part.copy(img);
        for(size_t i=1;i<=pa.size();++i)
        {
            pa[i]->transfer_contour(part, named_color::fetch( pa.size()+i*i ));
        }

        PNG.save("image_part.png",part,NULL);

        pa.regroup_all();
        



    }


}
YOCTO_UNIT_TEST_DONE()
