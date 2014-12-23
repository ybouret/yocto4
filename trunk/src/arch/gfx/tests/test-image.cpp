#include "yocto/utest/run.hpp"
#include "yocto/gfx/image.hpp"
#include "yocto/gfx/pixmap.hpp"
#include "yocto/gfx/image/jpeg.hpp"
#include "yocto/gfx/image/png.hpp"
#include "yocto/ptr/auto.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(image)
{
    gfx::image &IMG = gfx::image::instance();
    
    std::cerr << IMG.get_name() << std::endl;
    
    IMG.declare( new gfx::jpeg_format() );
    IMG.declare( new gfx::png_format()  );
    for(int i=1;i<argc;++i)
    {
        auto_ptr<gfx::surface> surf32( IMG.load_surface(argv[i], gfx::ARGB32() ) );
    }

}
YOCTO_UNIT_TEST_DONE()



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

        jpg.save_surface("img32.jpg", *surf32,NULL);
        jpg.save_surface("img16.jpg", *surf16,NULL);

    }
}
YOCTO_UNIT_TEST_DONE()

YOCTO_UNIT_TEST_IMPL(load_png)
{
    if(argc>1)
    {
        const string                filename = argv[1];
        const gfx::png_format       png;
        auto_ptr<gfx::surface>      surf32( png.load_surface(filename, gfx::ARGB32() ) );
        auto_ptr<gfx::surface>      surf16( png.load_surface(filename, gfx::ARGB16() ) );
        const gfx::jpeg_format      jpg;
        jpg.save_surface("toto32.jpg",  *surf32,NULL);
        png.save_surface("toto32.png",  *surf32,NULL);
        png.save_surface("toto16a.png", *surf16, "alpha");
    }

}
YOCTO_UNIT_TEST_DONE()

