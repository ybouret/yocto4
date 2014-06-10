#include "yocto/utest/run.hpp"
#include "yocto/gfx/pixel-format.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(format)
{
    gfx::pixel_format fmt(0x00ff0000,
                          0x0000ff00,
                          0x000000ff,
                          0xff000000);
    
    gfx::pixel_format fmt1 = gfx::RGBA32();
    std::cerr << fmt1 << std::endl;
    
    gfx::pixel_format fmt2 = gfx::ARGB32();
    std::cerr << fmt2 << std::endl;
    
    gfx::pixel_format fmt3 = gfx::RGB24();
    std::cerr << fmt3 << std::endl;
    
    gfx::pixel_format fmt4 = gfx::RGBA16();
    std::cerr << fmt4 << std::endl;
    
    gfx::pixel_format fmt5 = gfx::ARGB16();
    std::cerr << fmt5 << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()
