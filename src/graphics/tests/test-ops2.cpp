
#include "yocto/utest/run.hpp"
#include "yocto/graphics/ops/gradient.hpp"
#include "yocto/graphics/ops/samples.hpp"

#include "yocto/graphics/image/png.hpp"
#include "yocto/graphics/image/jpeg.hpp"
#include "yocto/graphics/image/tiff.hpp"

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

    if(argc>1)
    {
        const string filename = argv[1];
        pixmap4      pxm( IMG.load4(filename, NULL));
        PNG.save("image.png",pxm,NULL);

        const unit_t w = pxm.w;
        const unit_t h = pxm.h;
        xpatches xps;
        xpatch::create(xps, pxm, &server);

        std::cerr << "w=" << w << std::endl;
        std::cerr << "h=" << h << std::endl;
        std::cerr << "#patches=" << xps.size() << std::endl;

        pixmaps<uint8_t> ch(3,w,h);


        {
            std::cerr << "--- split channels..." << std::endl;
            samples S;
            S.split(ch,pxm,xps,&server);
            server.flush();

            get_red   get_r;
            get_green get_g;
            get_blue  get_b;
            PNG.save("image_r.png", ch[0], get_r, NULL);
            PNG.save("image_g.png", ch[1], get_g, NULL);
            PNG.save("image_b.png", ch[2], get_b, NULL);
        }


        {
            pixmap<float> g(w,h);

            gradient G;
            G.start(g,ch[0], xps, NULL);
            
            server.flush();
            PNG.save("image_grad_r.png",g, NULL);

        }

    }

}
YOCTO_UNIT_TEST_DONE()
