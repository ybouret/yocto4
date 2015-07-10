#include "yocto/utest/run.hpp"
#include "yocto/gfx/image/jpeg.hpp"
#include "yocto/gfx/image/png.hpp"
#include "yocto/gfx/image/tiff.hpp"
#include "yocto/gfx/rawpix.hpp"
#include "yocto/gfx/ops/split-channels.hpp"

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

YOCTO_UNIT_TEST_IMPL(img)
{
    image &IMG = image::instance();

    IMG.declare( new png_format() );
    IMG.declare( new jpeg_format() );
    IMG.declare( new tiff_format() );

    show_format( IMG["PNG"]  );
    show_format( IMG["JPEG"] );
    show_format( IMG["TIFF"] );

    if(argc>1)
    {
        const string    filename = argv[1];

        {
            bitmap::pointer bmp(IMG.load(filename,3, put_rgba::to_rgb,NULL,NULL));
            pixmap3         pxm(bmp,NULL);
            save_ppm("image3.ppm",pxm);
            pixmaps<uint8_t> px(3,pxm.w,pxm.h);
        }

        {
            bitmap::pointer bmp(IMG.load(filename,4, put_rgba::to_gsf,NULL,NULL));
            pixmapf         pxm(bmp,NULL);
            save_ppm("imagef.ppm",pxm);
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
        std::cerr << "Found #image=" << nd << std::endl;
        for(uint32_t i=0;i<nd;++i)
        {
            bitmap::pointer bmp( fmt->load_bitmap(filename, 3,  put_rgba::to_rgb, NULL,i) );
            pixmap3         pxm(bmp,NULL);
            
        }
    }
}
YOCTO_UNIT_TEST_DONE()