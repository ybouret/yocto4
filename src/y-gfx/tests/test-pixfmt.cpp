#include "yocto/utest/run.hpp"
#include "yocto/gfx/pixel-format.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/gfx/named-rgb.hpp"

using namespace yocto;


#define _SHOW(NAME) \
std::cerr << #NAME << "=" << gfx::conv::to_binary(fmt.mask.NAME) \
<< " | bits=" << int(fmt.bits.NAME)   \
<< " | shift=" << int(fmt.shift.NAME) \
<< " | loss =" << int(fmt.loss.NAME)  \
<< std::endl


#define _MAP(NAME) do { \
const gfx::rgb_t NAME = gfx::get_named_rgb(#NAME); \
std::cerr << "(" << int(NAME.r) << "," << int(NAME.g) << "," << int(NAME.b) << ") "; \
const gfx::pixel_t c  = fmt.map_rgb(NAME); \
std::cerr << #NAME << " = " << gfx::conv::to_binary(c) << " / " << gfx::conv::to_binary(c & (~fmt.opaque) ); \
const gfx::rgb_t tmp = fmt.get_rgb(c); \
std::cerr << "(" << int(tmp.r) << "," << int(tmp.g) << "," << int(tmp.b) << ") "; \
std::cerr << std::endl; \
} while(false)

static inline void display( const gfx::pixel_format &fmt )
{
    std::cerr << "bytes=" << fmt.bytes << std::endl;
    _SHOW(r);
    _SHOW(g);
    _SHOW(b);
    _SHOW(a);
    std::cerr << "\tbpp=" << fmt.bpp << std::endl;
    
    _MAP(red);
    _MAP(green);
    _MAP(blue);
    _MAP(yellow);

}



YOCTO_UNIT_TEST_IMPL(pixfmt)
{
        
    std::cerr << "checking colors" << std::endl;
    vector<string> colors;
    gfx::get_named_rgb(colors);
    
    for(size_t i=1; i <= colors.size(); ++i )
    {
        gfx::rgb_t col = gfx::get_named_rgb(colors[i]);
        std::cerr << colors[i] << " ==> " << int(col.r) << " " << int(col.g) << " " << int(col.b) << std::endl;
    }
    
    
    gfx::pixel_format fmt1(4,0xff000000,0x00ff0000,0x0000ff00,0x000000ff);
    display(fmt1);
        
    
    gfx::pixel_format fmt2(4,0x000f,0x00f0,0x0f00,0xf000);
    display(fmt2);
    
    
    
    
}
YOCTO_UNIT_TEST_DONE()
