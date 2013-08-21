#include "yocto/utest/run.hpp"
#include "yocto/gfx/surface.hpp"

using namespace yocto;


static inline
void display( const gfx::surface &S )
{
    std::cerr << "BPP    =" << S.format.bytes_per_pixel << std::endl;
    std::cerr << "width  =" << S.w << std::endl;
    std::cerr << "height =" << S.h << std::endl;
    std::cerr << "stride =" << S.pixels->stride << std::endl;
    std::cerr << std::endl;
}

YOCTO_UNIT_TEST_IMPL(surf)
{
    
    gfx::surface S1( gfx::color_format::ARGB32(), 100, 80);
    display(S1);
    
    gfx::surface S2( gfx::color_format::ARGB16(), 100, 80);
    display(S2);
    
    gfx::surface S3( gfx::color_format::RGB24(), 100, 80);
    display(S3);

    
}
YOCTO_UNIT_TEST_DONE()
