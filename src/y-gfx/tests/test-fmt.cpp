#include "yocto/utest/run.hpp"
#include "yocto/gfx/color-format.hpp"

using namespace yocto;


#define _SHOW(NAME) \
std::cerr << #NAME << "=" << gfx::conv::to_binary(fmt.mask.NAME) \
<< " | bits=" << int(fmt.bits.NAME)   \
<< " | shift=" << int(fmt.shift.NAME) \
<< " | loss =" << int(fmt.loss.NAME)  \
<< std::endl

static inline void display( const gfx::color_format &fmt )
{
    std::cerr << "bytes_per_pixel=" << fmt.bytes_per_pixel << std::endl;
    _SHOW(r);
    _SHOW(g);
    _SHOW(b);
    _SHOW(a);
    std::cerr << "\tbits_per_pixel=" << fmt.bits_per_pixel << std::endl;
}

YOCTO_UNIT_TEST_IMPL(fmt)
{
    gfx::color_format fmt1(4,0xff000000,0x00ff0000,0x0000ff00,0x000000ff);
    display(fmt1);

    gfx::color_format fmt2(4,0x000f,0x00f0,0x0f00,0xf000);
    display(fmt2);
    
}
YOCTO_UNIT_TEST_DONE()
