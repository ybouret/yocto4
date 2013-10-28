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
    std::cerr << "sizeof(pixmap<int>)=" << sizeof(gfx::pixmap<int>) << std::endl;
    std::cerr << "sizeof(pixmap<double>)=" << sizeof(gfx::pixmap<double>) << std::endl;

    gfx::pixmap<uint32_t> pix(500,100);
    
    gfx::bitmap::pointer bmp( gfx::bitmap::create(4,200,100) );
    
    const gfx::region rect(gfx::region(10,10,100,50)  );
    
    gfx::pixmap<uint32_t> pix2( bmp, rect);
    gfx::pixmap<float>    pix3( bmp, rect);
    gfx::pixmap<RGB4>     pix4(256,128);
    gfx::pixmap<YUV4>     pix5(256,128);

    
}
YOCTO_UNIT_TEST_DONE()
