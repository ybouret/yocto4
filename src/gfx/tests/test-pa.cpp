#include "yocto/gfx/image.hpp"
#include "yocto/gfx/image/jpeg.hpp"
#include "yocto/gfx/image/png.hpp"
#include "yocto/gfx/image/tiff.hpp"

#include "yocto/gfx/ops/filter.hpp"
#include "yocto/gfx/ops/differential.hpp"
#include "yocto/gfx/ops/histogram.hpp"
#include "yocto/gfx/ops/particles.hpp"
#include "yocto/gfx/ops/edges.hpp"
#include "yocto/gfx/ops/transform.hpp"

#include "yocto/gfx/color/named-colors.hpp"

#include "yocto/utest/run.hpp"
#include "yocto/string/conv.hpp"

using namespace yocto;
using namespace gfx;

static unit_t y_limit = 0;

static inline bool is_upper_vertex(const vertex &v) throw()
{
    return v.y>=y_limit;
}


YOCTO_UNIT_TEST_IMPL(pa)
{

    YOCTO_GFX_DECL_FORMAT(jpeg);
    YOCTO_GFX_DECL_FORMAT(png);
    YOCTO_GFX_DECL_FORMAT(tiff);

    imageIO          &IMG = image::instance();
    differential      drvs;

    if(argc>1)
    {
        size_t ns = 0;
        if(argc>2)
        {
            ns = strconv::to<size_t>(argv[2],"#smooth");
        }
        const string filename = argv[1];
        pixmap3      img( IMG.load3(filename,NULL) );
        xpatches     xps(img,true);

        IMG.save("img.png",img,0);

        const unit_t w=img.w;
        const unit_t h=img.h;
        Filter<RGB>  F(w,h);

        std::cerr << "#Smoothing=" << ns << std::endl;
        for(size_t i=0;i<ns;++i)
        {
            std::cerr << ".";
            F.Smooth(img,xps);
        }
        std::cerr << std::endl;
        const string suffix = ".png";
        IMG.save("img_smooth" + suffix,img,0);

        std::cerr << "-- Keep Foreground" << std::endl;
        pixmap3 fg(w,h);
        separate(threshold::keep_foreground,fg,img,xps);

        std::cerr << "-- Build Blobs" << std::endl;
        tagmap tmap(w,h);

        tmap.build(fg,8);
        IMG.save("img_tags0" + suffix, tmap, tmap.colors, 0 );

        std::cerr << "-- Load Particles" << std::endl;
        particles pa;
        pa.load(tmap);

        std::cerr << "-- Removing Upper Particles..." << std::endl;
        y_limit = h/2;
        pa.reject_all_vertices_from(tmap,is_upper_vertex);
        std::cerr << "#particles=" << pa.size() << std::endl;
        IMG.save("img_tags1"+suffix, tmap, tmap.colors, NULL);
        

        std::cerr << "-- Removing Shallow..." << std::endl;
        pa.remove_shallow_with(tmap);
        std::cerr << "#particles=" << pa.size() << std::endl;

        IMG.save("img_tags2"+suffix, tmap, tmap.colors, NULL);


        pixmap3 wksp(w,h);
        for(size_t i=1;i<=pa.size();++i)
        {
            pa[i]->transfer(wksp,img);
        }
        IMG.save("img_wksp"+suffix, wksp, NULL);




    }

}
YOCTO_UNIT_TEST_DONE()

