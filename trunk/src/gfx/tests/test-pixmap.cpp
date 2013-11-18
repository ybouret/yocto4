#include "yocto/utest/run.hpp"
#include "yocto/gfx/pixmap.hpp"


using namespace yocto;


namespace
{
    struct RGB4  { uint8_t r,g,b,a; };
    struct YUV4  { float   y,u,v,a; };
    
}

YOCTO_UNIT_TEST_IMPL(pixmap)
{
    std::cerr << "sizeof(pixmap<int>)   = " << sizeof(gfx::pixmap<int>) << std::endl;
    std::cerr << "sizeof(pixmap<double>)= " << sizeof(gfx::pixmap<double>) << std::endl;
    
    typedef gfx::pixmap<uint32_t> pixmap32;
    pixmap32::pointer pix1 = pixmap32::create(500,100);
    
    gfx::bitmap::pointer bmp( gfx::bitmap::create(4,200,100) );
    
    const gfx::region rect(gfx::region(10,10,100,50)  );
    
    gfx::pixmap<uint32_t>::pointer pix2 = pixmap32::attach( *bmp, &rect);
    gfx::pixmap<float>::pointer    pix3 = gfx::pixmap<float>::attach( *bmp, &rect);
    gfx::pixmap<RGB4>::pointer     pix4 = gfx::pixmap<RGB4>::create(256,128);
    gfx::pixmap<YUV4>::pointer     pix5 = gfx::pixmap<YUV4>::create(256,128);
    gfx::pixmap<float>::pointer    pix6 = gfx::pixmap<float>::carbon( *pix3, NULL);
    
}
YOCTO_UNIT_TEST_DONE()
