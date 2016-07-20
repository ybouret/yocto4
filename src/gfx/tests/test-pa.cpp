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

static inline uint8_t f2u(const float f, const float gamma)
{
    return gist::float2byte( powf(f,gamma) );
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
        xpatches     xps(img, new threading::engine(4,0,true) );

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

        std::cerr << "Building Edges" << std::endl;
        edges Edges(w,h);
        Edges.build_from(img,xps);
        IMG.save("img_edges" + suffix,Edges,0);
        IMG.save("img_edevs" + suffix,Edges.S,0);

        std::cerr << "Projecting..." << std::endl;
        transform       trans;
        pixmap<uint8_t> edges_mask(w,h);
        trans.apply(edges_mask, f2u, Edges, 2.0f, xps);
        IMG.save("img_edges_mask2" + suffix,edges_mask,0);

        trans.apply(edges_mask, f2u, Edges, 1.0f, xps);
        IMG.save("img_edges_mask" + suffix,edges_mask,0);



        pixmap<uint8_t> edevs_mask(w,h);
        trans.apply(edevs_mask, gist::float2byte, Edges.S,xps);
        IMG.save("img_edevs_mask" + suffix,edevs_mask,0);

        std::cerr << "Thresholding..." << std::endl;
        pixmap<uint8_t> edges_fg(w,h);
        separate(threshold::keep_foreground,edges_fg,edges_mask,xps);
        pixmap<uint8_t> edevs_fg(w,h);
        separate(threshold::keep_foreground,edevs_fg,edevs_mask,xps);

        IMG.save("img_edges_fg" + suffix,edges_fg,0);
        IMG.save("img_edevs_fg" + suffix,edevs_fg,0);


        std::cerr << "-- Build Tags..." << std::endl;

        tagmap tmap(w,h);

        tmap.build(edges_fg,8);
        std::cerr << "#current=" << tmap.current << std::endl;
        indx2rgba<size_t> tagColors(YGFX_RED);
        IMG.save("img_tags" +suffix, tmap, tagColors, NULL);

        

    }

}
YOCTO_UNIT_TEST_DONE()

