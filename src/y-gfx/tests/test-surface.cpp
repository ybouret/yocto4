#include "yocto/utest/run.hpp"
#include "yocto/gfx/surface.hpp"


using namespace yocto;



YOCTO_UNIT_TEST_IMPL(surface)
{
    std::cerr << "sizeof(gfx::surface)=" << sizeof(gfx::surface) << std::endl;
    gfx::surface::pointer s = gfx::surface::create( gfx::format::RGB24(), 100, 50);

    
}
YOCTO_UNIT_TEST_DONE()
