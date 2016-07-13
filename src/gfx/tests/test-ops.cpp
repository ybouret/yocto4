#include "yocto/gfx/image.hpp"
#include "yocto/gfx/image/jpeg.hpp"
#include "yocto/gfx/image/png.hpp"
#include "yocto/gfx/image/tiff.hpp"

#include "yocto/gfx/ops/samples.hpp"
#include "yocto/gfx/ops/histogram.hpp"
#include "yocto/gfx/ops/blur.hpp"
#include "yocto/gfx/ops/fft.hpp"
#include "yocto/gfx/color/orange.hpp"
#include "yocto/gfx/color/blue_to_red.hpp"
#include "yocto/gfx/color/cold_to_hot.hpp"

#include "yocto/code/rand.hpp"

#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace gfx;

static inline void save_with_ramp(ramp                &rmp,
                                  const pixmap<float> &px,
                                  const string        &filename,
                                  const imageIO       &IMG)
{
    rmp.vmax=rmp.vmin=px[0][0];
    for(unit_t j=0;j<px.h;++j)
    {
        for(unit_t i=0;i<px.w;++i)
        {
            const float tmp = px[j][i];
            rmp.vmin = min_of(tmp,rmp.vmin);
            rmp.vmax = max_of(tmp,rmp.vmax);
        }
    }
    IMG.save(filename,px,rmp,NULL);
}

static inline void save_with_ramp(ramp                &rmp,
                                  const pixmapz       &px,
                                  const string        &filename,
                                  const imageIO       &IMG)
{
    pixmap<float> rpx(px.w,px.h);
    rmp.vmax=rmp.vmin=px[0][0].mod();
    for(unit_t j=0;j<px.h;++j)
    {
        for(unit_t i=0;i<px.w;++i)
        {
            const float tmp = px[j][i].mod();
            rmp.vmin = min_of(tmp,rmp.vmin);
            rmp.vmax = max_of(tmp,rmp.vmax);
            rpx[j][i] = tmp;
        }
    }
    IMG.save(filename,rpx,rmp,NULL);
}



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

        orange u_ramp;
        save_with_ramp(u_ramp, fch[1], "img_u.png", IMG);

        blue_to_red v_ramp;
        save_with_ramp(v_ramp, fch[2], "img_v.png", IMG);



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

        const unit_t ww = next_power_of_two(w);
        const unit_t hh = next_power_of_two(h);
        pixmapz zimg(ww,hh);
        fourier::forward(zimg,igs);
        cold_to_hot z_ramp;
        save_with_ramp(z_ramp,zimg, "img_fft.png", IMG);


    }
}
YOCTO_UNIT_TEST_DONE()
