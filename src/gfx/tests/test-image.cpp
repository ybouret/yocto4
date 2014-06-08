#include "yocto/utest/run.hpp"
#include "yocto/gfx/image.hpp"
#include "yocto/gfx/pixmap.hpp"
#include "yocto/gfx/image/jpeg.hpp"
#include "yocto/gfx/image/png.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(image)
{
    gfx::image &IMG = gfx::image::instance();
    
    std::cerr << IMG.get_name() << std::endl;
    
    IMG.declare( new gfx::jpeg_format() );
    IMG.declare( new gfx::png_format()  );

}
YOCTO_UNIT_TEST_DONE()


#include "yocto/ptr/auto.hpp"

YOCTO_UNIT_TEST_IMPL(load_jpeg)
{
    if(argc>1)
    {
        const string                filename = argv[1];
        const gfx::jpeg_format      jpg;
        auto_ptr<gfx::surface>      surf32( jpg.load_surface(filename, gfx::ARGB32() ) );
        auto_ptr<gfx::surface>      surf16( jpg.load_surface(filename, gfx::ARGB16() ) );

        gfx::pixmap<gfx::rgb_t>     pix3(   jpg.load_bitmap3(filename) );
        gfx::pixmap<gfx::rgba_t>    pix4(   jpg.load_bitmap4(filename) );
        gfx::pixmap<uint8_t>        pixgs(  jpg.load_greyscale(filename) );
        gfx::pixmap<float>          pixf(   jpg.load_greyscale_f(filename) );

        jpg.save_surface("toto32.jpg", *surf32,NULL);
        jpg.save_surface("toto16.jpg", *surf16,NULL);

    }
}
YOCTO_UNIT_TEST_DONE()

YOCTO_UNIT_TEST_IMPL(load_png)
{
    if(argc>1)
    {
        const string                filename = argv[1];
        const gfx::png_format       png;
        png.load(filename, 3, NULL, NULL);
        
    }

}
YOCTO_UNIT_TEST_DONE()

