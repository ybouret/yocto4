#include "yocto/utest/run.hpp"
#include "yocto/gfx/surface.hpp"
#include "yocto/gfx/named-rgb.hpp"

using namespace yocto;

namespace  {
    
    static inline
    void fill_surface( gfx::surface &s )
    {
        
        std::cerr << "Surface " << s.w << "x" << s.h << ", depth=" << s.depth << ", bpp=" << s.bpp << std::endl;
        
        const gfx::rgb_t col[] =
        {
            gfx::get_named_rgb("blue"),
            gfx::get_named_rgb("red"),
            gfx::get_named_rgb("green")
        };
        
        const size_t num = sizeof(col)/sizeof(col[0]);
        
        size_t k=0;
        for( gfx::unit_t y=0;y<s.h;++y)
        {
            for( gfx::unit_t x=0;x<s.w;++x)
            {
                const gfx::rgb_t &c = col[(k++)%num];
                s.put_pixel( s[y][x], s.map_rgb(c.r, c.g, c.b) );
            }
        }
    }
    
}

YOCTO_UNIT_TEST_IMPL(surface)
{
    std::cerr << "sizeof(gfx::surface)=" << sizeof(gfx::surface) << std::endl;
    gfx::surface::pointer s1 = gfx::surface::create( gfx::format::RGB24(), 100, 50);
    fill_surface(*s1);

    gfx::surface::pointer s2 = gfx::surface::create( gfx::format::ARGB32(), 100, 50);
    fill_surface(*s2);
    
    gfx::surface::pointer s3 = gfx::surface::create( gfx::format::ARGB16(), 100, 50);
    fill_surface(*s3);
    
}
YOCTO_UNIT_TEST_DONE()
