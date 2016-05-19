
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

        {
            std::cerr << "--- split channels..." << std::endl;
            samples S;
            std::cerr << "\tsequential" << std::endl;
            YOCTO_TIMINGS(tmx, DURATION, S.split(ch,pxm,seq,NULL));
            const double split_seq = tmx.speed;
            std::cerr << "\tsplit_seq=" << split_seq << " fps" << std::endl;

            std::cerr << "\tparallel" << std::endl;
            YOCTO_TIMINGS(tmx,DURATION,S.split(ch,pxm,xps,&server));
            const double split_par = tmx.speed;
            std::cerr << "\tsplit_par=" << split_par << " fps" << std::endl;


            PNG.save("image_r.png", ch[0], get_r, NULL);
            PNG.save("image_g.png", ch[1], get_g, NULL);
            PNG.save("image_b.png", ch[2], get_b, NULL);
        }


        {
            std::cerr << "--- compute gradient..." << std::endl;
            pixmap<float> g(w,h);

            gradient G;
            G.compute(ch[0],g,ch[0], xps, NULL);
            PNG.save("image_grad_r.png",ch[0], get_r, NULL);

            G.compute(ch[1],g,ch[1], xps, NULL);
            PNG.save("image_grad_g.png",ch[1], get_g, NULL);

            G.compute(ch[2],g,ch[2], xps, NULL);
            PNG.save("image_grad_b.png",ch[2], get_b, NULL);
        }

        pixmap4 bmp(w,h);
        {
            std::cerr << "--- merge channels..." << std::endl;
            samples S;
            S.merge(ch,bmp,xps,&server);
            PNG.save("image_grad.png",bmp,NULL);
        }

        histogram H;
        {
            std::cerr << "--- compute histogram..." << std::endl;
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
