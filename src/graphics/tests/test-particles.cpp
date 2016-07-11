#include "yocto/utest/run.hpp"

#include "yocto/graphics/image/png.hpp"
#include "yocto/graphics/image/jpeg.hpp"
#include "yocto/graphics/image/tiff.hpp"
#include "yocto/graphics/ops/histogram.hpp"
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

    threading::engine server(true);

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
            std::cerr << "-- Filtering.." << std::endl;
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
        tmap.build(fg);

        PNG.save("image_tag.png",tmap,tmap.to_rgba,NULL);

        std::cerr << "-- Create initial particles" << std::endl;
        particles pa;
        pa.load(tmap);

        
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
        for(size_t i=1;i<=pa.size();++i)
        {
            break;
            particle &p = *pa[i];
            std::cerr << "#pixels =" << p.size << std::endl;
            std::cerr << "|_AABB  =" << p.compute_extension() << std::endl;
        }

        std::cerr << "-- Make one fusion" << std::endl;
        std::cerr << "#start_particle=" << pa.size() << std::endl;
        pa.dilate_and_join(tmap);
        std::cerr << "#final_particle=" << pa.size() << std::endl;

        part.copy(img);
        for(size_t i=1;i<=pa.size();++i)
        {
            pa[i]->transfer_contour(part, named_color::fetch( pa.size()+i*i ));
        }
        PNG.save("image_part2.png",part,NULL);
        PNG.save("image_tag2.png",tmap,tmap.to_rgba,NULL);


        return 0;

        {
            std::cerr << "Full Erosion..." << std::endl;
            while(pa.erode_and_check(tmap)>0)
            {
                break;
            }

            pa.regroup_all();
            pa.split_all_using(tmap);
            part.copy(img);
            for(size_t i=1;i<=pa.size();++i)
            {
                pa[i]->transfer_contour(part, named_color::fetch( pa.size()+i*i ));
            }
            PNG.save("image_part2.png",part,NULL);

        }

        pa.discard(10,tmap);



    }


}
YOCTO_UNIT_TEST_DONE()
