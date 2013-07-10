#include "yocto/utest/run.hpp"
#include "yocto/gfx/surface.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;


static inline
void test_surf( gfx::surface &surf )
{
    for(size_t j=0;j<surf.h;++j)
    {
        for(size_t i=0;i<surf.w;++i)
        {
            void    *p = surf[j][i];
            uint32_t c = _rand.full<uint32_t>();
            surf.format.set_pixel(c, p);
        }
    }
    
}

YOCTO_UNIT_TEST_IMPL(surface)
{
    const gfx::color_format fmt = gfx::color_format::RGBA32();
    gfx::surface            surf(fmt,512,256);    
    test_surf(surf);
    surf.save_ppm("surf.ppm");
}
YOCTO_UNIT_TEST_DONE()
