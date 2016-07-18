#include "yocto/gfx/image.hpp"
#include "yocto/gfx/image/jpeg.hpp"
#include "yocto/gfx/image/png.hpp"
#include "yocto/gfx/image/tiff.hpp"

#include "yocto/gfx/ops/filter.hpp"
#include "yocto/gfx/ops/differential.hpp"
#include "yocto/gfx/ops/histogram.hpp"
#include "yocto/gfx/ops/particles.hpp"

#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace gfx;

YOCTO_UNIT_TEST_IMPL(pa)
{

    YOCTO_GFX_DECL_FORMAT(jpeg);
    YOCTO_GFX_DECL_FORMAT(png);
    YOCTO_GFX_DECL_FORMAT(tiff);

    imageIO          &IMG = image::instance();
    threading::engine server(4,0,true);
    xpatches          xps;
    differential      drvs;

    if(argc>1)
    {
        const string filename = argv[1];
        pixmap3      img( IMG.load3(filename,NULL) );
        xpatch::create(xps, img, &server);

        IMG.save("img.png",img,0);

        const unit_t w=img.w;
        const unit_t h=img.h;

        std::cerr << "-- Gradient..." << std::endl;
        pixmap3      grd(w,h);
        {
            pixmaps<real_t> channels(3,w,h);
            drvs.compute(grd,img,channels,differential::gradient, xps, &server);
        }
        IMG.save("img_grd.png",grd,0);

        std::cerr << "-- Foreground..." << std::endl;
        pixmap3     fg(w,h);
        separate(threshold::keep_foreground,fg,grd,xps,&server);

        IMG.save("img_fg.png",fg,0);

        std::cerr << "-- Build Tags..." << std::endl;

        tagmap tmap(w,h);

        tmap.build(fg,8);
        std::cerr << "#current=" << tmap.current << std::endl;

    }

}
YOCTO_UNIT_TEST_DONE()

