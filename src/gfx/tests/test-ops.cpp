#include "yocto/gfx/image.hpp"
#include "yocto/gfx/image/jpeg.hpp"
#include "yocto/gfx/image/png.hpp"
#include "yocto/gfx/image/tiff.hpp"

#include "yocto/gfx/ops/samples.hpp"
#include "yocto/gfx/ops/histogram.hpp"
#include "yocto/gfx/ops/blur.hpp"
#include "yocto/gfx/ops/fft.hpp"
#include "yocto/gfx/ops/filter.hpp"
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

template <typename T>
static inline void apply_filters(const pixmap<T>   &src,
                                 const string      &id,
                                 xpatches           xps,
                                 threading::engine *server)
{
    //const unit_t w = src.w;
    //const unit_t h = src.h;
    imageIO          &IMG = image::instance();


    pixmap<T> org(src);
    pixmap<T> dst(src);

    filter<T> F;
    std::cerr << "-- Filtering " << id << " average" << std::endl;
    for(unsigned i=1;i<=4;++i)
    {
        org.copy(dst);
        F.apply(dst,org,&F, &filter<T>::average,xps,server);
        const string filename = "img_" + id + vformat("_ave%u.png",i);
        IMG.save(filename,dst,NULL);
    }

    std::cerr << "-- Filtering " << id << " median" << std::endl;
    dst.copy(src);
    for(unsigned i=1;i<=4;++i)
    {
        org.copy(dst);
        F.apply(dst,org,&F, &filter<T>::median,xps,server);
        const string filename = "img_" + id + vformat("_med%u.png",i);
        IMG.save(filename,dst,NULL);
    }

    std::cerr << "-- Filtering " << id << " dilate" << std::endl;
    dst.copy(src);
    for(unsigned i=1;i<=4;++i)
    {
        org.copy(dst);
        F.apply(dst,org,&F, &filter<T>::dilate,xps,server);
        const string filename = "img_" + id + vformat("_dilate%u.png",i);
        IMG.save(filename,dst,NULL);
    }

    std::cerr << "-- Filtering " << id << " erode" << std::endl;
    dst.copy(src);
    for(unsigned i=1;i<=4;++i)
    {
        org.copy(dst);
        F.apply(dst,org,&F, &filter<T>::erode,xps,server);
        const string filename = "img_" + id + vformat("_erode%u.png",i);
        IMG.save(filename,dst,NULL);
    }




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

        std::cerr << "-- FFT..." << std::endl;
        const unit_t ww = next_power_of_two(w);
        const unit_t hh = next_power_of_two(h);
        pixmapz zimg(ww,hh);
        fourier::forward(zimg,igs);
        cold_to_hot z_ramp;
        save_with_ramp(z_ramp,zimg, "img_fft.png", IMG);

        std::cerr << "-- iFFT..." << std::endl;
        fourier::reverse(zimg);
        pixmapf frev(w,h);
        fourier::transfer(frev,zimg);
        IMG.save("img_rev.png",frev,NULL);

        std::cerr << "-- Filter..." << std::endl;

        pixmap4 img4(w,h);
        pixmap1 img1(w,h);
        for(unit_t j=0;j<h;++j)
        {
            for(unit_t i=0;i<w;++i)
            {
                const RGB &C = img[j][i];
                img4[j][i] = C;
                img1[j][i] = gist::greyscale1(C.r,C.g,C.b);
            }
        }

        apply_filters(img,"rgb"   , xps,&server);
        apply_filters(igs,"gs",     xps,&server);
        apply_filters(img4,"rgba",  xps,&server);
        apply_filters(img1,"u",     xps,&server);



    }
}
YOCTO_UNIT_TEST_DONE()
