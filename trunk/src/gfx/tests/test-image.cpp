#include "yocto/utest/run.hpp"
#include "yocto/gfx/image.hpp"
#include "yocto/gfx/pixmap.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(image)
{
    gfx::image &IMG = gfx::image::instance();
    
    std::cerr << IMG.get_name() << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()


#include "yocto/gfx/image/jpeg.hpp"
#include "yocto/ptr/auto.hpp"

YOCTO_UNIT_TEST_IMPL(load_jpeg)
{
    if(argc>1)
    {
        const string                filename = argv[1];
        const gfx::jpeg_format      jpg;
        auto_ptr<gfx::surface>      surf32( jpg.load_surface(filename, gfx::ARGB32() ) );
        
#if 0
        gfx::pixmap<gfx::rgb_t>     pix3(   jpg.load_bitmap3(filename) );
        gfx::pixmap<gfx::rgba_t>    pix4(   jpg.load_bitmap4(filename) );
        gfx::pixmap<uint8_t>        pixgs(  jpg.load_greyscale(filename) );
        gfx::pixmap<float>          pixf(   jpg.load_greyscale_f(filename) );
#endif
        jpg.save_surface("toto.jpg", *surf32,NULL);
        
    }
}
YOCTO_UNIT_TEST_DONE()
