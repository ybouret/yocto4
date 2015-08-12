#include "yocto/utest/run.hpp"
#include "yocto/gfx/image/jpeg.hpp"
#include "yocto/gfx/image/png.hpp"
#include "yocto/gfx/image/tiff.hpp"
#include "yocto/gfx/rawpix.hpp"
#include "yocto/gfx/ops/stencil.hpp"
#include "yocto/gfx/ops/gradient.hpp"
#include "yocto/gfx/ops/split-channels.hpp"
#include "yocto/gfx/ops/histogram.hpp"

using namespace yocto;
using namespace gfx;

#define SHOW_SIZE_OF(N,TYPE) std::cerr << "sizeof(stencil<" #N "," #TYPE ">)\t=\t" << sizeof(stencil<N,TYPE>) << std::endl

YOCTO_UNIT_TEST_IMPL(stencil)
{
    image &IMG = image::instance();
    
    IMG.declare( new png_format()  );
    IMG.declare( new jpeg_format() );
    IMG.declare( new tiff_format() );
    
    const image::format &PNG = IMG["PNG"];
    
    SHOW_SIZE_OF(0,uint8_t);
    SHOW_SIZE_OF(1,uint8_t);
    SHOW_SIZE_OF(2,uint8_t);
    
    SHOW_SIZE_OF(0,float);
    SHOW_SIZE_OF(1,float);
    SHOW_SIZE_OF(2,float);
    
    if(argc>1)
    {
        const string         filename = argv[1];
        
        {
            bitmap::pointer bmp(IMG.load(filename,3, put_rgba::to_rgb,NULL,NULL));
            pixmap3         pxm(bmp,NULL);
            PNG.save("image3.png",pxm, get_rgba::from_rgb,NULL, NULL);
            
            // pxf: black and white
            pixmapf pxf(pxm,rgb2bwf<rgb_t>);
            PNG.save("imagec.png",pxf, get_rgba::from_rampf,NULL, NULL);
            
            
            pixmaps<uint8_t> ch(3,pxm.w,pxm.h);
            split_channels(ch,pxm);
            PNG.save("image_col_r.png",ch[0], get_rgba::from_byte_r,NULL, NULL);
            PNG.save("image_col_g.png",ch[1], get_rgba::from_byte_g,NULL, NULL);
            PNG.save("image_col_b.png",ch[2], get_rgba::from_byte_b,NULL, NULL);
            gradient(ch[0], ch[0]);
            gradient(ch[1], ch[1]);
            gradient(ch[2], ch[2]);
            PNG.save("grad_image_r.png",ch[0], get_rgba::from_byte_r,NULL, NULL);
            PNG.save("grad_image_g.png",ch[1], get_rgba::from_byte_g,NULL, NULL);
            PNG.save("grad_image_b.png",ch[2], get_rgba::from_byte_b,NULL, NULL);
            gradient(pxf, pxf);
            PNG.save("grad_image_bw.png",pxf, get_rgba::from_gsf,NULL, NULL);
            
            merge_channels(ch, pxm);
            PNG.save("grad_image3.png",pxm, get_rgba::from_rgb,NULL, NULL);

        }
        
    }
    
}
YOCTO_UNIT_TEST_DONE()
