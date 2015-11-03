#include "yocto/graphics/image/png.hpp"
#include "yocto/graphics/image/jpeg.hpp"
#include "yocto/graphics/image/tiff.hpp"
#include "yocto/graphics/ops/hist.hpp"
#include "yocto/graphics/ops/blob.hpp"
#include "yocto/graphics/ops/channels.hpp"
#include "yocto/graphics/ops/grad.hpp"
#include "yocto/graphics/ops/stencil.hpp"
#include "yocto/graphics/ops/blur.hpp"
#include "yocto/ios/ocstream.hpp"

#include "yocto/utest/run.hpp"

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
    
    std::cerr << "blur::amplitude=" << blur::amplitude << std::endl;
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
        
        {
            pixmap3 gc(pxm.w,pxm.h);
            channels::merge(ch,gc, chp, &server);
            PNG.save("grad3.png", gc, NULL);
        }
        
        std::cerr << "Stencils..." << std::endl;
        stencil::weights W;
        W(0,0)  = -4;
        W(1,0)  = W(0,1) = W(-1,0) = W(0,-1) = 1;
        
        std::cerr << "stencil=" << W << std::endl;
        stencil::patches sp;
        std::cerr << "pgs.size=" << pgs.w << "x" << pgs.h << std::endl;
        stencil::create(sp, pgs, &server);
        
        stencil::launch(sp, g, W, pgs, gist::fill_with_zero, &server);
        pixmapf dest(pxm.w,pxm.h);
        
        stencil::finish(sp, g, dest, &server);
        PNG.save("sten.png",dest,NULL);
        
        
        std::cerr << "Blur..." << std::endl;
        blur::patches blr;
        blur::create(blr,pgs,&server);
        blur::launch(blr,g,pgs,2.0f, &server);
        blur::finish(blr,g,dest,&server);
        PNG.save("blur.png",dest,NULL);

        
        
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
        
        vector<histogram::cbin> cdf;
        uint8_t lut[256];
        H.build_cdf(cdf,lut);
        {
            ios::wcstream fp("cdf.dat");
            for(size_t i=1;i<=cdf.size();++i)
            {
                fp("%g %g\n", double(cdf[i].x), double(cdf[i].y) );
            }
        }
        {
            ios::wcstream fp("lut.dat");
            for(size_t i=0;i<256;++i)
            {
                fp("%d %d\n",int(i),int(lut[i]));
            }
        }
    }
}
YOCTO_UNIT_TEST_DONE()
