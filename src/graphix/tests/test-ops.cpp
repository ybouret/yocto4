#include "yocto/graphix/image/png.hpp"
#include "yocto/graphix/image/jpeg.hpp"
#include "yocto/graphix/image/tiff.hpp"
#include "yocto/graphix/ops/hist.hpp"
#include "yocto/graphix/ops/blob.hpp"

#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace graphix;

YOCTO_UNIT_TEST_IMPL(ops)
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
        PNG.save("image4.png",pxm,NULL);

        histogram H;
        H.update(pxm);
        H.save("hist.dat");

        H.reset();
        histogram::patches hp;
        histogram::create(hp,pxm,NULL);
        histogram::launch(hp,pxm,NULL);
        H.finish(hp,NULL);
        H.save("hist2.dat");

        H.reset();
        histogram::create(hp,pxm,&server);
        histogram::launch(hp,pxm,&server);
        H.finish(hp,&server);
        H.save("hist3.dat");


        const size_t t = H.threshold();
        std::cerr << "Threshold@" << t << std::endl;

        pixmap4 tgt(pxm.w,pxm.h);
        threshold::apply(tgt,t,pxm,threshold::keep_background);
        PNG.save("bg.png", tgt, NULL);
        threshold::apply(tgt,t,pxm,threshold::keep_foreground);
        PNG.save("fg.png", tgt, NULL);

        get_named_color<blob::type> bproc;

        blob Blob(pxm.w,pxm.h,&server);
        Blob.__detect(tgt,8);
        PNG.save("blob_a.png",Blob, bproc, NULL);

        const size_t nb = Blob.__reduce(8,&server);
        std::cerr << "nb=" << nb << std::endl;
        PNG.save("blob_b.png",Blob, bproc, NULL);



        vector<size_t> sizes;
        Blob.__format(sizes);
        std::cerr << "sizes=" << sizes << std::endl;
    }
}
YOCTO_UNIT_TEST_DONE()
