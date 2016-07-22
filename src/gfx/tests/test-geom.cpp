#include "yocto/gfx/image.hpp"
#include "yocto/gfx/image/jpeg.hpp"
#include "yocto/gfx/image/png.hpp"
#include "yocto/gfx/image/tiff.hpp"

#include "yocto/gfx/ops/geometry.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/math/trigconv.hpp"
#include "yocto/sys/timings.hpp"

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
        const pixmap3 img( IMG.load3(filename,NULL) );
        IMG.save("img.png",img,NULL);
        xpatches xps(img,true);
        const unit_t w=img.w;
        const unit_t h=img.h;

        geometry    geom;
        pixmap3     tgt(w,h);
        timings     tmx;

#define DURATION 1
        std::cerr << "Rotation..." << std::endl;
        YOCTO_TIMINGS(tmx,DURATION,geom.rotate(tgt,img,vertex(w/2,h/2), math::Deg2Rad(15.0f),xps));
        std::cerr << "speed=" << tmx.speed << std::endl;
        IMG.save("img_rot.png",tgt,NULL);

        std::cerr << "Sym/X" << std::endl;
        YOCTO_TIMINGS(tmx,DURATION,geom.symmetry(tgt, img, -1.0f,0.0f,0.55f*float(w),xps));
        std::cerr << "speed=" << tmx.speed << std::endl;
        IMG.save("img_symx.png",tgt,NULL);

        std::cerr << "Sym/Y" << std::endl;
        YOCTO_TIMINGS(tmx,DURATION,geom.symmetry(tgt, img, 0.0f,-1.0f,0.55f*float(h),xps));
        std::cerr << "speed=" << tmx.speed << std::endl;
        IMG.save("img_symy.png",tgt,NULL);

        std::cerr << "Sym/XY" << std::endl;
        YOCTO_TIMINGS(tmx,DURATION,geom.symmetry(tgt, img, 1.0f,-1.0f,0.0f,xps));
        std::cerr << "speed=" << tmx.speed << std::endl;
        IMG.save("img_symm.png",tgt,NULL);


    }


}
YOCTO_UNIT_TEST_DONE()

#include "yocto/math/opt/cgrad.hpp"
#include "yocto/gfx/color/named-colors.hpp"
#include "yocto/gfx/ops/symmetry-finder.hpp"

YOCTO_UNIT_TEST_IMPL(symfind)
{

    YOCTO_GFX_DECL_FORMAT(jpeg);
    YOCTO_GFX_DECL_FORMAT(png);
    YOCTO_GFX_DECL_FORMAT(tiff);

    imageIO          &IMG = image::instance();
    if(argc>1)
    {
        const string filename = argv[1];
        std::cerr << "-- Loading " << filename << std::endl;
        const pixmap3 img( IMG.load3(filename,NULL) );
        IMG.save("img.png",img, NULL);

        pixmap3       tgt(img.w,img.h);
        xpatches xps(img,true);

        symmetry_finder<RGB> sym(tgt,img,xps);
        math::cgrad<float>::callback cb( &sym, & symmetry_finder<RGB>::__callback);

        vector<float> aorg(2);
        vector<bool>  used(2,true);
        //used[1] = false;

        aorg[2] = -img.w/2+5;
        if(math::cgrad<float>::optimize(sym.scalarF,sym.vectorF, aorg, used, 1e-4, &cb))
        {
            std::cerr << "converged" << std::endl;
            std::cerr << aorg << std::endl;
            sym.scalarF(aorg);

            const float alpha = aorg[1];
            const float a     = cosf(alpha);
            const float b     = sinf(alpha);
            const float c     = aorg[2];

            const RGB Cm = named_color::fetch(YGFX_BLUE);
            const RGB Cp = named_color::fetch(YGFX_RED);
            for(unit_t j=0;j<img.h;++j)
            {
                for(unit_t i=0;i<img.w;++i)
                {
                    const RGB C = ( (a*float(i)+b*float(j)+c)<=0 ) ? Cm : Cp;
                    tgt[j][i] = pixel<RGB>::blend(tgt[j][i], C, 128);
                }
            }

            IMG.save("out.png",tgt, NULL);

        }
        else
        {
            std::cerr << "not converged..." << std::endl;
        }

    }

}
YOCTO_UNIT_TEST_DONE()


