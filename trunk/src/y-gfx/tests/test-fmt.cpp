#include "yocto/utest/run.hpp"
#include "yocto/gfx/color-format.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(fmt)
{
    {
        gfx::color_format fmt(4,0xff000000,0x00ff0000,0x0000ff00,0x000000ff);
    }
    
    {
        gfx::color_format fmt(4,0x0f000000,0x00ff0000,0x0000ff00,0x000000ff);
    }
    
    {
        std::cerr << std::endl << "RGBA32" << std::endl;
        gfx::color_format fmt( gfx::color_format::RGBA32());
    }
    
    {
        std::cerr << std::endl << "ARGB32" << std::endl;
        gfx::color_format fmt( gfx::color_format::ARGB32());
    }
    
    {
        std::cerr << std::endl << "RGB24" << std::endl;
        gfx::color_format fmt( gfx::color_format::RGB24());
    }
    
    {
        std::cerr << std::endl << "RGBA16" << std::endl;
        gfx::color_format fmt( gfx::color_format::RGBA16());
    }
    
}
YOCTO_UNIT_TEST_DONE()

