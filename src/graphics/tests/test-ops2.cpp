
#include "yocto/utest/run.hpp"
#include "yocto/graphics/ops/gradient.hpp"
#include "yocto/graphics/ops/samples.hpp"
#include "yocto/graphics/ops/histogram.hpp"

#include "yocto/graphics/image/png.hpp"
#include "yocto/graphics/image/jpeg.hpp"
#include "yocto/graphics/image/tiff.hpp"

#include "yocto/sys/timings.hpp"

using namespace yocto;
using namespace graphics;

YOCTO_UNIT_TEST_IMPL(ops2)
{
    image &IMG = image::instance();

    IMG.declare( new png_format()  );
    IMG.declare( new jpeg_format() );
    IMG.declare( new tiff_format() );

    const image::format &PNG = IMG["PNG"];


    threading::engine server(true);
    timings           tmx;

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

        bool check_speedup = true;

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
            pixmap<float> g(w,h);
            gradient G;

            if(check_speedup)
            {
                std::cerr << "\tsequential" << std::endl;
                YOCTO_TIMINGS(tmx, DURATION, G.compute(ch[0],g,ch[0], seq, NULL));
                const double grad_seq = tmx.speed;
                std::cerr << "\tgrad_seq=" << grad_seq << " fps" << std::endl;

                std::cerr << "\tparallel" << std::endl;
                YOCTO_TIMINGS(tmx, DURATION, G.compute(ch[0],g,ch[0], xps, &server));
                const double grad_par = tmx.speed;
                std::cerr << "\tgrad_par=" << grad_par << " fps" << std::endl;
                std::cerr << "\t\tspeedup=" << grad_par/grad_seq << std::endl;

            }

            G.compute(ch[0],g,ch[0], xps, &server);
            PNG.save("image_grad_r.png",ch[0], get_r, NULL);

            G.compute(ch[1],g,ch[1], xps, &server);
            PNG.save("image_grad_g.png",ch[1], get_g, NULL);

            G.compute(ch[2],g,ch[2], xps, &server);
            PNG.save("image_grad_b.png",ch[2], get_b, NULL);
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
            threshold::apply(bmp, t, pxm, threshold::keep_background);
            PNG.save("image_bg.png", bmp, NULL);
        }
    }
    
}
YOCTO_UNIT_TEST_DONE()
