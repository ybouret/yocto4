#include "yocto/graphix/image/png.hpp"
#include "yocto/graphix/image/jpeg.hpp"
#include "yocto/graphix/image/tiff.hpp"
#include "yocto/graphix/ops/hist.hpp"
#include "yocto/graphix/ops/blob.hpp"
#include "yocto/graphix/ops/channels.hpp"
#include "yocto/graphix/ops/grad.hpp"

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

        pixmapf     pgs( pxm, to_float<RGBA>, pxm);
        PNG.save("imageGS.png",pgs,NULL);

        std::cerr << "SplitChannels..." << std::endl;
        pixmaps<uint8_t>  ch(3,pxm.w,pxm.h);
        channels::patches chp;
        channels::create(chp,pxm, &server);
        channels::split(ch,pxm,chp,&server);

        std::cerr << "Gradients" << std::endl;
        gradient::patches gp;
        gradient::create(gp,pxm, &server);
        pixmap<gradient::real_type> g(pxm.w,pxm.h);
        gradient::launch(g,pgs,gp,&server);
        pixmapf grad(pxm.w,pxm.h);
        gradient::finish(grad,g,gp,&server);
        PNG.save("gradGS.png", grad, NULL);


        get_red   get_r;
        get_green get_g;
        get_blue  get_b;
        PNG.save("image_r.png", ch[0], get_r, NULL);
        PNG.save("image_g.png", ch[1], get_g, NULL);
        PNG.save("image_b.png", ch[2], get_b, NULL);

        for(size_t i=0;i<3;++i)
        {
            gradient::launch(g,ch[i], gp, &server);
            gradient::finish(ch[i],g,gp, &server);
        }
        PNG.save("grad_r.png", ch[0], get_r, NULL);
        PNG.save("grad_g.png", ch[1], get_g, NULL);
        PNG.save("grad_b.png", ch[2], get_b, NULL);
        
        std::cerr << "Histograms" << std::endl;
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

        get_named_color<blob::type> bproc;
        vector<size_t> blobs;
        blob Blob(pxm.w,pxm.h,&server);

        pixmap4 tgt(pxm.w,pxm.h);
        threshold::apply(tgt,t,pxm,threshold::keep_background);
        PNG.save("bg.png", tgt, NULL);

        std::cerr << "BackGround Blobs.." << std::endl;
        Blob.__detect(tgt,8);
        PNG.save("bg_blob_a.png",Blob,bproc,NULL);
        Blob.__reduce(8,&server);
        PNG.save("bg_blob_b.png",Blob,bproc,NULL);
        Blob.__format(blobs,&server,1);
        PNG.save("bg_blob_c.png",Blob,bproc,NULL);

        if(blobs.size()>=1)
        {
            pixmap1 b1(pxm.w,pxm.h);
            pixmap4 b4(pxm.w,pxm.h);
            Blob.transfer<uint8_t>(1, b1, 255);
            Blob.transfer(1,b4,pxm);
            PNG.save("bg_blob1.png", b1, NULL);
            PNG.save("bg_blob4.png", b4, NULL);

        }


        threshold::apply(tgt,t,pxm,threshold::keep_foreground);
        PNG.save("fg.png", tgt, NULL);
        std::cerr << "ForeGround Blobs.." << std::endl;
        Blob.__detect(tgt,8);
        PNG.save("fg_blob_a.png",Blob,bproc,NULL);
        Blob.__reduce(8,&server);
        PNG.save("fg_blob_b.png",Blob,bproc,NULL);
        Blob.__format(blobs,&server,1);
        PNG.save("fg_blob_c.png",Blob,bproc,NULL);
        if(blobs.size()>=1)
        {
            pixmap1 b1(pxm.w,pxm.h);
            pixmap4 b4(pxm.w,pxm.h);
            Blob.transfer<uint8_t>(1, b1, 255);
            Blob.transfer(1,b4,pxm);
            PNG.save("fg_blob1.png", b1, NULL);
            PNG.save("fg_blob4.png", b4, NULL);
            
        }
        
        
    }
}
YOCTO_UNIT_TEST_DONE()
