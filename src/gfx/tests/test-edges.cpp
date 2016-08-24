#include "yocto/gfx/image.hpp"
#include "yocto/gfx/image/jpeg.hpp"
#include "yocto/gfx/image/png.hpp"
#include "yocto/gfx/image/tiff.hpp"

#include "yocto/gfx/ops/edges.hpp"

#include "yocto/code/rand.hpp"

#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace gfx;

YOCTO_UNIT_TEST_IMPL(edges)
{

    YOCTO_GFX_DECL_FORMAT(jpeg);
    YOCTO_GFX_DECL_FORMAT(png);
    YOCTO_GFX_DECL_FORMAT(tiff);

    imageIO          &IMG = image::instance();
    if(argc>1)
    {
        std::cerr << "-- Load" << std::endl;
        const string filename = argv[1];
        pixmapf      img( IMG.loadf(filename,NULL) );
        IMG.save("img-f.png",img,NULL);
        xpatches xps(img,true);

        const unit_t w=img.w;
        const unit_t h=img.h;
        EdgeDetector ED(w,h);

        const stencil_grad_x  gx;
        const stencil_grad_y  gy;
        const stencil_sobel_x sx;
        const stencil_sobel_y sy;

        ED.build_from(img, gx, gy, xps);
        IMG.save("img-grad.png",ED,0);

        ED.build_from(img, sx, sy, xps);
        IMG.save("img-sobel.png",ED,0);

    }

}
YOCTO_UNIT_TEST_DONE()

