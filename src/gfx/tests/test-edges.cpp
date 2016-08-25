#include "yocto/gfx/image.hpp"
#include "yocto/gfx/image/jpeg.hpp"
#include "yocto/gfx/image/png.hpp"
#include "yocto/gfx/image/tiff.hpp"

#include "yocto/gfx/ops/edges.hpp"
#include "yocto/gfx/color/ramp/cold_to_very_hot.hpp"

#include "yocto/code/rand.hpp"
#include "yocto/string/conv.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace gfx;

YOCTO_UNIT_TEST_IMPL(edges)
{

    YOCTO_GFX_DECL_FORMAT(jpeg);
    YOCTO_GFX_DECL_FORMAT(png);
    YOCTO_GFX_DECL_FORMAT(tiff);

    cold_to_very_hot rmp;
    rmp.vmin = -3.15;
    rmp.vmax =  3.15;

    imageIO          &IMG = image::instance();
    if(argc>1)
    {
        std::cerr << "-- Load" << std::endl;
        const string  filename = argv[1];
        const pixmapf img( IMG.loadf(filename,NULL) );
        IMG.save("img-f.png",img,NULL);
        xpatches xps(img,true);


        const unit_t w=img.w;
        const unit_t h=img.h;
        EdgeDetector        ED(w,h);

        float sig = 1.4f;
        if(argc>2)
        {
            sig = strconv::to<float>(argv[2],"sig");
        }

        pixmapf img3(w,h);
        pixmapf img5(w,h);
        {
            const stencil_gauss   g3(1,sig);
            const stencil_gauss   g5(2,sig);
            stencil::dispatcher   dsp(w,h);
            dsp(g3,img3,img,xps);
            dsp(g5,img5,img,xps);
        }
        IMG.save("img-3.png",img3,NULL);
        IMG.save("img-5.png",img5,NULL);

        const stencil_grad_x  gx;
        const stencil_grad_y  gy;
        const stencil_sobel_x sx;
        const stencil_sobel_y sy;

        ED.build_from(img, gx, gy, xps);
        IMG.save("img-grad-f.png",ED,0);
        ED.build_from(img3, gx, gy, xps);
        IMG.save("img-grad-3.png",ED,0);
        ED.build_from(img5, gx, gy, xps);
        IMG.save("img-grad-5.png",ED,0);





    }

}
YOCTO_UNIT_TEST_DONE()

