#include "yocto/utest/run.hpp"
#include "yocto/gfx/pixmap.hpp"


using namespace yocto;



YOCTO_UNIT_TEST_IMPL(pixmap)
{
    std::cerr << "sizeof(pixmap<int>)=" << sizeof(gfx::pixmap<int>) << std::endl;
    std::cerr << "sizeof(pixmap<double>)=" << sizeof(gfx::pixmap<double>) << std::endl;

    gfx::pixmap<uint32_t> pix(500,100);
    
    gfx::bitmap::pointer bmp( gfx::bitmap::create(4,200,100) );
    
    const gfx::region rect(gfx::region(10,10,100,50)  );
    
    gfx::pixmap<uint32_t> pix2( bmp, rect);
    gfx::pixmap<float>    pix3( bmp, rect);
    
    
    
}
YOCTO_UNIT_TEST_DONE()
