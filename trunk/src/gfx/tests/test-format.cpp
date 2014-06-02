#include "yocto/utest/run.hpp"
#include "yocto/gfx/format.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(format)
{
    gfx::format fmt(0x00ff0000,
                    0x0000ff00,
                    0x000000ff,
                    0xff000000);
    
    gfx::format fmt1 = gfx::format::RGBA32();
    std::cerr << fmt1 << std::endl;
    
    gfx::format fmt2 = gfx::format::ARGB32();
    std::cerr << fmt2 << std::endl;
    
    gfx::format fmt3 = gfx::format::RGB24();
    std::cerr << fmt3 << std::endl;
    
    gfx::format fmt4 = gfx::format::RGBA16();
    std::cerr << fmt4 << std::endl;

    gfx::format fmt5 = gfx::format::ARGB16();
    std::cerr << fmt5 << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()
