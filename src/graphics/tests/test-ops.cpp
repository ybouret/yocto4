
#include "yocto/utest/run.hpp"
#include "yocto/graphics/ops/gradient.hpp"
#include "yocto/graphics/ops/samples.hpp"
#include "yocto/graphics/ops/histogram.hpp"
#include "yocto/graphics/ops/blobs.hpp"
#include "yocto/graphics/ops/stencil.hpp"
#include "yocto/graphics/ops/fft.hpp"
#include "yocto/graphics/ops/blur.hpp"

#include "yocto/graphics/image/png.hpp"
#include "yocto/graphics/image/jpeg.hpp"
#include "yocto/graphics/image/tiff.hpp"

#include "yocto/sys/timings.hpp"

using namespace yocto;
using namespace graphics;

YOCTO_UNIT_TEST_IMPL(ops)
{
    image &IMG = image::instance();

    IMG.declare( new png_format()  );
    IMG.declare( new jpeg_format() );
    IMG.declare( new tiff_format() );

    const image::format &PNG = IMG["PNG"];


    threading::engine server(true);
    timings           tmx;

    stencil S;
    S[-1][-1] = 2;
    std::cerr << "S=" << S << std::endl;


    if(argc>1)
    {
        const string filename = argv[1];
        pixmap4      pxm( IMG.load4(filename, NULL));
        PNG.save("image.png",pxm,NULL);

        const unit_t w = pxm.w;
        const unit_t h = pxm.h;
        xpatches xps;
        xpatch::create(xps, pxm, &server);

        xpatches seq;
        xpatch::create(seq,pxm, NULL);


        std::cerr << "w=" << w << std::endl;
        std::cerr << "h=" << h << std::endl;
        std::cerr << "#patches=" << xps.size() << std::endl;

        pixmaps<uint8_t> ch(3,w,h);


        get_red   get_r;
        get_green get_g;
        get_blue  get_b;
#define DURATION 3

        bool check_speedup = false;


        {
            std::cerr << "--- split channels..." << std::endl;
            samples S;
            if(check_speedup)
            {
                std::cerr << "\tsequential" << std::endl;
                YOCTO_TIMINGS(tmx, DURATION, S.split(ch,pxm,seq,NULL));
                const double split_seq = tmx.speed;
                std::cerr << "\tsplit_seq=" << split_seq << " fps" << std::endl;

                std::cerr << "\tparallel" << std::endl;
                YOCTO_TIMINGS(tmx,DURATION,S.split(ch,pxm,xps,&server));
                const double split_par = tmx.speed;
                std::cerr << "\tsplit_par=" << split_par << " fps" << std::endl;

                std::cerr << "\t\tspeedup=" << split_par/split_seq << std::endl;

            }

            S.split(ch,pxm,xps,&server);
            PNG.save("image_r.png", ch[0], get_r, NULL);
            PNG.save("image_g.png", ch[1], get_g, NULL);
            PNG.save("image_b.png", ch[2], get_b, NULL);
        }


        {
            std::cerr << "--- compute gradient..." << std::endl;
            pixmap<float> gr(w,h);
            pixmap<float> gg(w,h);
            pixmap<float> gb(w,h);

            gradient G;

            if(check_speedup)
            {
                std::cerr << "\tsequential" << std::endl;
                YOCTO_TIMINGS(tmx, DURATION, G.compute(ch[0],gr,ch[0], seq, NULL));
                const double grad_seq = tmx.speed;
                std::cerr << "\tgrad_seq=" << grad_seq << " fps" << std::endl;

                std::cerr << "\tparallel" << std::endl;
                YOCTO_TIMINGS(tmx, DURATION, G.compute(ch[0],gr,ch[0], xps, &server));
                const double grad_par = tmx.speed;
                std::cerr << "\tgrad_par=" << grad_par << " fps" << std::endl;
                std::cerr << "\t\tspeedup=" << grad_par/grad_seq << std::endl;

            }

            const float mr = G.compute(ch[0],gr,ch[0], xps, &server);
            PNG.save("image_grad_r.png",ch[0], get_r, NULL);

            const float mg = G.compute(ch[1],gg,ch[1], xps, &server);
            PNG.save("image_grad_g.png",ch[1], get_g, NULL);

            const float mb = G.compute(ch[2],gb,ch[2], xps, &server);
            PNG.save("image_grad_b.png",ch[2], get_b, NULL);

            pixmap3 GG(w,h);
            const float mc = max_of( max_of(mr,mg), mb );
            if(mc>0)
            {
                const float gfac = 1.0 / mc;
                for(unit_t j=0;j<h;++j)
                {
                    for(unit_t i=0;i<w;++i)
                    {
                        RGB C(gist::float2byte(gr[j][i]*gfac),
                              gist::float2byte(gg[j][i]*gfac),
                              gist::float2byte(gb[j][i]*gfac));
                        GG[j][i] = C;
                    }
                }
            }
            PNG.save("image_grad_rgb.png",GG, NULL);


        }

        pixmap4 bmp(w,h);
        {
            std::cerr << "--- merge channels..." << std::endl;
            samples S;

            if(check_speedup)
            {
                std::cerr << "\tsequential" << std::endl;
                YOCTO_TIMINGS(tmx, DURATION, S.merge(ch,bmp,seq,NULL) );
                const double merge_seq = tmx.speed;
                std::cerr << "\tmerge_seq=" << merge_seq << " fps" << std::endl;


                std::cerr << "\tparallel" << std::endl;
                YOCTO_TIMINGS(tmx, DURATION, S.merge(ch,bmp,xps,&server) );
                const double merge_par = tmx.speed;
                std::cerr << "\tmerge_par=" << merge_par << " fps" << std::endl;

                std::cerr << "\t\tspeedup=" << merge_par/merge_seq << std::endl;
            }

            S.merge(ch,bmp,xps,&server);
            PNG.save("image_grad.png",bmp,NULL);
        }

        histogram H;
        blobs     B(w,h);
        {
            std::cerr << "--- compute histogram..." << std::endl;
            if(check_speedup)
            {
                H.reset();
                std::cerr << "\tsequential" << std::endl;
                YOCTO_TIMINGS(tmx, DURATION, H.update(pxm,seq,NULL));
                const double hist_seq = tmx.speed;
                std::cerr << "\thist_seq=" << hist_seq << " fps" << std::endl;

                H.reset();
                std::cerr << "\tparallel" << std::endl;
                YOCTO_TIMINGS(tmx, DURATION, H.update(pxm,xps,&server));
                const double hist_par = tmx.speed;
                std::cerr << "\thist_par=" << hist_par << " fps" << std::endl;
                std::cerr << "\t\tspeedup=" << hist_par/hist_seq << std::endl;

            }

            H.reset();
            H.update(pxm,xps,&server);
            H.save("hist.dat");
            const size_t t = H.threshold();
            std::cerr << "threshold=" << t << std::endl;
            threshold::apply(bmp, t, pxm, threshold::keep_foreground);
            PNG.save("image_fg.png", bmp, NULL);

            get_named_color<size_t> blobColors;
            std::cerr << "-- blobs from foreground..." << std::endl;
            B.build(bmp);
            std::cerr << "#blobs=" << B.current << std::endl;
            PNG.save("image_fg_blobs.png", B, blobColors, NULL);

            std::cerr << "#fg_content=" << B.content.size() << std::endl;
            for(size_t i=1;i<=B.content.size();++i)
            {
                std::cerr << "\tfg_blob[" << i << "] => " << B.content[i]->size << " | tag=" << B.content[i]->tag << std::endl;
            }

            pixmap4 tgt(w,h);
            if(B.content.size()>0)
            {
                const blob &big = *B.content[1];
                big.transfer(tgt,bmp);
            }
            PNG.save("image_fg_big.png",tgt,NULL);

            threshold::apply(bmp, t, pxm, threshold::keep_background);
            PNG.save("image_bg.png", bmp, NULL);
            std::cerr << "-- blobs from background..." << std::endl;
            B.build(bmp);
            std::cerr << "#blobs=" << B.current << std::endl;
            PNG.save("image_bg_blobs.png", B, blobColors, NULL);
            std::cerr << "#bg_content=" << B.content.size() << std::endl;
            for(size_t i=1;i<=B.content.size();++i)
            {
                std::cerr << "\tbg_blob[" << i << "] => " << B.content[i]->size << " | tag=" << B.content[i]->tag << std::endl;
            }

        }

        std::cerr << "-- testing fourier..." << std::endl;
        pixmapf pgs(bmp,to_float<RGBA>,bmp);
        PNG.save("image_gs.png", pgs, NULL);

        pixmapz pzm( next_power_of_two(w), next_power_of_two(h) );
        fourier::forward(pzm,pgs);


        {
            pixmapz pzm2(pzm);
            fourier::reverse(pzm2);
            pgs.ldz();
            fourier::transfer(pgs, pzm2);
            PNG.save("image_gs_fft_rev.png",pgs,NULL);
        }


        {
            float cmx = 0;
            for(unit_t j=0;j<h;++j)
            {
                for(unit_t i=0;i<w;++i)
                {
                    cmx = max_of(cmx, pzm[j][i].mod());
                }
            }
            std::cerr << "cmx=" << cmx << std::endl;
            if(cmx>0)
            {
                for(unit_t j=0;j<h;++j)
                {
                    for(unit_t i=0;i<w;++i)
                    {
                        pzm[j][i] /= cmx;
                    }
                }
            }
        }
        get_gsz zproc;
        PNG.save("image_fft.png", pzm, zproc, NULL);

        fourier::reverse(pzm);
        {
            float cmx = 0;
            for(unit_t j=0;j<h;++j)
            {
                for(unit_t i=0;i<w;++i)
                {
                    cmx = max_of(cmx, pzm[j][i].mod());
                }
            }
            std::cerr << "cmx=" << cmx << std::endl;
            if(cmx>0)
            {
                for(unit_t j=0;j<h;++j)
                {
                    for(unit_t i=0;i<w;++i)
                    {
                        pzm[j][i] /= cmx;
                    }
                }
            }
        }
        PNG.save("image_ifft.png", pzm, zproc, NULL);

        
        std::cerr << "-- blur" << std::endl;
        
        pixmap4 blr(w,h);
        
        for(int sig=1;sig<=5;++sig)
        {
            std::cerr << "\tsig=" << sig << std::endl;
            blur bf(sig);
            std::cerr << "\t\twidth=" << bf.width << std::endl;
            {
                const string outname = vformat("mask_blur%02d.png",sig);
                PNG.save(outname, bf, NULL);
            }
            {
                bf.apply<RGBA,uint8_t,3>(blr,pxm,xps, &server);
                const string outname = vformat("image_blur%02d.png",sig);
                PNG.save(outname, blr, NULL);
            }
        }
        
    }
    
}
YOCTO_UNIT_TEST_DONE()
