#include "yocto/gfx/image.hpp"
#include "yocto/gfx/image/jpeg.hpp"
#include "yocto/gfx/image/png.hpp"
#include "yocto/gfx/image/tiff.hpp"

#include "yocto/gfx/ops/stencil.hpp"
#include "yocto/gfx/color/ramp/cold_to_very_hot.hpp"
#include "yocto/gfx/color/ramp/blue_to_red.hpp"
#include "yocto/gfx/color/ramp/grey.hpp"

#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace gfx;

static inline void display_stencil( stencil &S, const char *id)
{
    std::cerr << "raw " << id << ":"; S.display();
    //S.optimize();
    //std::cerr << "opt " << id << ":"; S.display();
}

YOCTO_UNIT_TEST_IMPL(stencil)
{
    YOCTO_GFX_DECL_FORMAT(jpeg);
    YOCTO_GFX_DECL_FORMAT(png);
    YOCTO_GFX_DECL_FORMAT(tiff);


    imageIO          &IMG = image::instance();
    blue_to_red       rmp;
    grey              grmp;

    if(argc>1)
    {

        const string filename = argv[1];
        pixmap3      img3( IMG.load3(filename,NULL) );
        pixmapf      imgf( IMG.loadf(filename,NULL) );
        IMG.save("img-f.png",imgf,0);

        xpatches     xps(imgf,true);

        stencil_grad_x gx;
        stencil_grad_y gy;

        const unit_t w = imgf.w;
        const unit_t h = imgf.h;

        pixmapf      tgt(w,h);

        std::cerr << "grad x" << std::endl;
        gx.apply(tgt,imgf,xps);
        rmp.vmin = gx.global.vmin;
        rmp.vmax = gx.global.vmax;
        IMG.save("img-gx.png",tgt,rmp,0);

        std::cerr << "grad y" << std::endl;
        gy.apply(tgt,imgf,xps);
        rmp.vmin = gy.global.vmin;
        rmp.vmax = gy.global.vmax;
        IMG.save("img-gy.png",tgt,rmp,0);

        std::cerr << "sobel x" << std::endl;
        stencil_sobel_x sx;
        sx.apply(tgt,imgf,xps);
        rmp.vmin = sx.global.vmin;
        rmp.vmax = sx.global.vmax;
        IMG.save("img-sx.png",tgt,rmp,0);

        std::cerr << "sobel y" << std::endl;
        stencil_sobel_y sy;
        sy.apply(tgt,imgf,xps);
        rmp.vmin = sy.global.vmin;
        rmp.vmax = sy.global.vmax;
        IMG.save("img-sy.png",tgt,rmp,0);

        std::cerr << "gauss 3x3" << std::endl;
        stencil_gauss g3(1,1.4f);
        g3.apply(tgt,imgf,xps);
        rmp.vmin = g3.global.vmin;
        rmp.vmax = g3.global.vmax;
        IMG.save("img-g3.png",tgt,grmp,0);

        std::cerr << "gauss 5x5" << std::endl;
        stencil_gauss g5(2,1.4f);
        g5.apply(tgt,imgf,xps);
        rmp.vmin = g5.global.vmin;
        rmp.vmax = g5.global.vmax;
        IMG.save("img-g5.png",tgt,grmp,0);

        //if(false)
        {
            display_stencil(gx,"grad_x");
            display_stencil(gy,"grad_y");
            display_stencil(sx,"sobel_x");
            display_stencil(sy,"sobel_y");
            display_stencil(g3,"g3x3");
            display_stencil(g5,"g5x5");
        }

        pixmap3 tgt3(w,h);

        stencil::dispatcher dsp(w,h);


        std::cerr << "grad x rgb..." << std::endl;
        dsp(gx,tgt3,img3,xps);
        IMG.save("img-gx-rgb.png",tgt3,0);

        std::cerr << "grad y rgb..." << std::endl;
        dsp(gy,tgt3,img3,xps);
        IMG.save("img-gy-rgb.png",tgt3,0);

        std::cerr << "gauss 3x3 rgb..." << std::endl;
        dsp(g3,tgt3,img3,xps);
        IMG.save("img-g3-rgb.png",tgt3,0);

        std::cerr << "gauss 5x5 rgb..." << std::endl;
        dsp(g5,tgt3,img3,xps);
        IMG.save("img-g5-rgb.png",tgt3,0);



    }
}
YOCTO_UNIT_TEST_DONE()