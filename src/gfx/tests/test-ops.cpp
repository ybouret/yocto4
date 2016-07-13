#include "yocto/gfx/image.hpp"
#include "yocto/gfx/image/jpeg.hpp"
#include "yocto/gfx/image/png.hpp"
#include "yocto/gfx/image/tiff.hpp"

#include "yocto/gfx/ops/samples.hpp"
#include "yocto/gfx/ops/histogram.hpp"
#include "yocto/gfx/ops/blur.hpp"

#include "yocto/code/rand.hpp"

#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace gfx;

YOCTO_UNIT_TEST_IMPL(ops)
{

    YOCTO_GFX_DECL_FORMAT(jpeg);
    YOCTO_GFX_DECL_FORMAT(png);
    YOCTO_GFX_DECL_FORMAT(tiff);

    imageIO          &IMG = image::instance();
    threading::engine server(true);
    xpatches          xps;
    if(argc>1)
    {
        std::cerr << "-- Load" << std::endl;
        const string filename = argv[1];
        pixmap3      img( IMG.load3(filename,NULL) );
        IMG.save("img.png",img,NULL);
        xpatch::create(xps, img, &server);

        std::cerr << "-- to YUV" << std::endl;
        yuvmap       yuv(img,YUV::fromRGB,img);
        const unit_t w = img.w;
        const unit_t h = img.h;

        std::cerr << "-- Split" << std::endl;
        samples<YUV>   yuv_samples;
        pixmaps<float> fch(3,w,h);

        yuv_samples.split(fch,yuv,xps,&server);

        IMG.save("img_y.png",fch[0],NULL);

        for(unit_t j=0;j<h;++j)
        {
            for(unit_t i=0;i<w;++i)
            {
                fch[0][j][i] *= 0.5f+0.3f*alea<float>();
            }
        }

        std::cerr << "-- Merge" << std::endl;
        yuv_samples.merge(fch,yuv,xps,&server);
        IMG.save("img2.png",yuv,NULL);

        std::cerr << "-- Greyscale..." << std::endl;
        pixmapf igs(img,RGB::to_float,img);
        IMG.save("igs.png",igs,NULL);

        std::cerr << "-- Keep Foreground RGB" << std::endl;
        pixmap3 fg3(w,h);
        separate(threshold::keep_foreground,fg3,img, xps, &server);
        IMG.save("fg3.png",fg3,NULL);

        std::cerr << "-- Keep Foreground Float" << std::endl;
        pixmapf fgf(w,h);
        separate(threshold::keep_foreground,fgf,igs, xps, &server);
        IMG.save("fgf.png",fgf,NULL);

        std::cerr << "-- Blur RGB" << std::endl;
        apply_blur(1.2, fg3, img, xps, &server);
        IMG.save("blur3.png", fg3, NULL);


        std::cerr << "-- Blur Float" << std::endl;
        apply_blur(1.2, fgf, igs, xps, &server);
        IMG.save("blurf.png", fgf, NULL);


    }
}
YOCTO_UNIT_TEST_DONE()
