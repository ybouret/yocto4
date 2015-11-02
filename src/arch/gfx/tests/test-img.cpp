#include "yocto/utest/run.hpp"
#include "yocto/gfx/image/jpeg.hpp"
#include "yocto/gfx/image/png.hpp"
#include "yocto/gfx/image/tiff.hpp"
#include "yocto/gfx/rawpix.hpp"
#include "yocto/gfx/ops/split-channels.hpp"
#include "yocto/gfx/ops/histogram.hpp"
#include "yocto/gfx/ops/stencil.hpp"

using namespace yocto;
using namespace gfx;

namespace
{

    static void show_format( const image::format &fmt )
    {
        std::cerr << "<" << fmt.name << ">" << std::endl;
        const char **exts = fmt.extensions();
        for(size_t i=0;;++i)
        {
            const char *ext = exts[i];
            if(!ext) break;
            std::cerr << "\tmay load " << ext << std::endl;
        }
    }

}

#include "yocto/ios/ocstream.hpp"

YOCTO_UNIT_TEST_IMPL(img)
{
    image &IMG = image::instance();

    IMG.declare( new png_format()  );
    IMG.declare( new jpeg_format() );
    IMG.declare( new tiff_format() );

    show_format( IMG["PNG"]  );
    show_format( IMG["JPEG"] );
    show_format( IMG["TIFF"] );

    const image::format &PNG = IMG["PNG"];

    if(argc>1)
    {
        const string    filename = argv[1];

        {
            bitmap::pointer bmp(IMG.load(filename,3, put_rgba::to_rgb,NULL,NULL));
            pixmap3         pxm(bmp,NULL);
            PNG.save("image3.png",pxm, get_rgba::from_rgb,NULL, NULL);
            pixmaps<uint8_t> ch(3,pxm.w,pxm.h);
            split_channels(ch,pxm);
            PNG.save("image_col_r.png",ch[0], get_rgba::from_byte_r,NULL, NULL);
            PNG.save("image_col_g.png",ch[1], get_rgba::from_byte_g,NULL, NULL);
            PNG.save("image_col_b.png",ch[2], get_rgba::from_byte_b,NULL, NULL);

            {
                histogram H;
                H.equalize8(ch[0],1.0);
                {
                    {
                        ios::wcstream fp("lut_r.dat");
                        for(size_t i=0;i<histogram::bins;++i)
                        {
                            fp("%d %d %d\n", int(i), int(H.lut[i]), int(H.count[i]) );
                        }
                    }

                }
                H.equalize8(ch[1],1.0);
                {
                    {
                        ios::wcstream fp("lut_g.dat");
                        for(size_t i=0;i<histogram::bins;++i)
                        {
                            fp("%d %d %d\n", int(i), int(H.lut[i]), int(H.count[i]) );
                        }
                    }
                }
                H.equalize8(ch[2],1.0);
                {
                    {
                        ios::wcstream fp("lut_b.dat");
                        for(size_t i=0;i<histogram::bins;++i)
                        {
                            fp("%d %d %d\n", int(i), int(H.lut[i]), int(H.count[i]) );
                        }
                    }
                }

                PNG.save("image_eq_r.png",ch[0], get_rgba::from_byte_r,NULL, NULL);
                PNG.save("image_eq_g.png",ch[1], get_rgba::from_byte_g,NULL, NULL);
                PNG.save("image_eq_b.png",ch[2], get_rgba::from_byte_b,NULL, NULL);


                pixmap3 peq(pxm.w,pxm.h);
                merge_channels(ch,peq);
                PNG.save("image3eq.png",peq, get_rgba::from_rgb,NULL, NULL);
            }


#if 1
            {
                histogram Hgs,Hbw;
                Hgs.build_for(pxm,rgb2gs8<rgb_t>);
                Hbw.build_for(pxm,rgb2bw8<rgb_t>);

                {
                    ios::wcstream fp("hist.dat");
                    for(int i=0;i<256;++i)
                    {
                        fp("%d %u %u\n", i, unsigned(Hgs.count[i]), unsigned(Hbw.count[i]));
                    }
                }

                {
                    ios::wcstream fp("cdf_gs.dat");
                    for(size_t i=0;i<Hgs.classes;++i)
                    {
                        fp("%g %g\n", double(Hgs.bin[i]),Hgs.cdf[i]);
                    }
                }

                {
                    ios::wcstream fp("cdf_bw.dat");
                    for(size_t i=0;i<Hbw.classes;++i)
                    {
                        fp("%g %g\n", double(Hbw.bin[i]),Hbw.cdf[i]);
                    }
                }

                const size_t gs_t = Hgs.threshold();
                const size_t bw_t = Hbw.threshold();
                std::cerr << "thresholds=" << gs_t << ", " << bw_t << std::endl;

                {
                    pixmap3 gs_fg(pxm.w,pxm.h),gs_bg(pxm.w,pxm.h);
                    threshold::apply(gs_fg, gs_t, pxm,rgb2gs8<rgb_t>, threshold::keep_foreground );
                    threshold::apply(gs_bg, gs_t, pxm,rgb2gs8<rgb_t>, threshold::keep_background );

                    PNG.save("image_cut_gs_fg.png",gs_fg, get_rgba::from_rgb,NULL, NULL);
                    PNG.save("image_cut_gs_bg.png",gs_bg, get_rgba::from_rgb,NULL, NULL);

                }


                {
                    pixmap3 bw_fg(pxm.w,pxm.h),bw_bg(pxm.w,pxm.h);
                    threshold::apply(bw_fg, bw_t, pxm,rgb2bw8<rgb_t>, threshold::keep_foreground );
                    threshold::apply(bw_bg, bw_t, pxm,rgb2bw8<rgb_t>, threshold::keep_background );

                    PNG.save("image_cut_bw_fg.png",bw_fg, get_rgba::from_rgb,NULL, NULL);
                    PNG.save("image_cut_bw_bg.png",bw_bg, get_rgba::from_rgb,NULL, NULL);
                }

                {
                    ios::wcstream fp("icdf.dat");
                    for(double y=0;y<=1;y+=0.01)
                    {
                        fp("%g %g %g %g\n", y, Hgs.icdf(y, 0.8), Hgs.icdf(y,1.0), Hgs.icdf(y, 1.2) );
                    }
                }

                Hgs.build_lut(1.0);
                Hbw.build_lut(1.0);
                {
                    ios::wcstream fp("lut.dat");
                    for(size_t i=0;i<histogram::bins;++i)
                    {
                        fp("%d %d %d\n", int(i), int(Hgs.lut[i]), int(Hbw.lut[i]));
                    }
                }

                {
                    pixmap1 eq_gs(pxm.w,pxm.h);
                    Hgs.apply_lut(eq_gs, to_byte<uint8_t>,pxm,rgb2gs8<rgb_t>);
                    save_ppm("image_lut_gs1.0.ppm",eq_gs);
                    Hgs.build_lut(0.4);
                    Hgs.apply_lut(eq_gs, to_byte<uint8_t>,pxm,rgb2gs8<rgb_t>);
                    save_ppm("image_lut_gs0.4.ppm",eq_gs);
                    Hgs.build_lut(2.2);
                    Hgs.apply_lut(eq_gs, to_byte<uint8_t>,pxm,rgb2gs8<rgb_t>);
                    save_ppm("image_lut_gs2.2.ppm",eq_gs);

                }

            }
#endif

        }

        {
            bitmap::pointer bmp(IMG.load(filename,4, put_rgba::to_gsf,NULL,NULL));
            pixmapf         pxm(bmp,NULL);
            save_ppm("image_gs.ppm",pxm);
        }

        {
            bitmap::pointer bmp(IMG.load(filename,4, put_rgba::to_bwf,NULL,NULL));
            pixmapf         pxm(bmp,NULL);
            save_ppm("image_bw.ppm",pxm);
        }


    }


}
YOCTO_UNIT_TEST_DONE()


YOCTO_UNIT_TEST_IMPL(tiff)
{
    arc_ptr<tiff_format>  fmt( new tiff_format() );
    if(argc>1)
    {
        const string    filename = argv[1];
        const uint32_t  nd       = fmt->count_directories(filename);
        //std::cerr << "Found #image=" << nd << std::endl;
        for(uint32_t i=0;i<nd;++i)
        {
            bitmap::pointer bmp( fmt->load_bitmap(filename, 3,  put_rgba::to_rgb, NULL,i) );
            pixmap3         pxm(bmp,NULL);
            
        }
    }
}
YOCTO_UNIT_TEST_DONE()

