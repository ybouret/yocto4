#include "yocto/utest/run.hpp"
#include "yocto/gfx/bitmap.hpp"


using namespace yocto;


YOCTO_UNIT_TEST_IMPL(bitmap)
{
    std::cerr << "sizeof(bitmap)=" << sizeof(gfx::bitmap) << std::endl;
    gfx::bitmap::pointer bmp( gfx::bitmap::create(4,200,100) );
    gfx::bitmap::pointer sub( gfx::bitmap::link(bmp, NULL)   );
    
}
YOCTO_UNIT_TEST_DONE()
