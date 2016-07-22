#include "yocto/gfx/image.hpp"
#include "yocto/gfx/image/jpeg.hpp"
#include "yocto/gfx/image/png.hpp"
#include "yocto/gfx/image/tiff.hpp"

#include "yocto/gfx/ops/geometry.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/math/trigconv.hpp"

using namespace yocto;
using namespace gfx;

YOCTO_UNIT_TEST_IMPL(geom)
{
    YOCTO_GFX_DECL_FORMAT(jpeg);
    YOCTO_GFX_DECL_FORMAT(png);
    YOCTO_GFX_DECL_FORMAT(tiff);

    imageIO          &IMG = image::instance();
    if(argc>1)
    {
        std::cerr << "-- Load" << std::endl;
        const string filename = argv[1];
        pixmap3      img( IMG.load3(filename,NULL) );
        IMG.save("img.png",img,NULL);
        xpatches xps(img,true);
        const unit_t w=img.w;
        const unit_t h=img.h;

        geometry geom;
        pixmap3     tgt(w,h);
        geom.rotate(tgt,img,vertex(w/2,h/2), math::Deg2Rad(15.0f),xps);
        IMG.save("img_rot.png",tgt,NULL);

    }


}
YOCTO_UNIT_TEST_DONE()

