#include "yocto/utest/run.hpp"
#include "yocto/gfx/ramp/types.hpp"
#include "yocto/gfx/ramp/r01.hpp"
#include "yocto/gfx/ramp/r02.hpp"
#include "yocto/gfx/ramp/r03.hpp"
#include "yocto/gfx/ramp/r04.hpp"
#include "yocto/gfx/ramp/r10.hpp"
#include "yocto/gfx/ramp/r14.hpp"
#include "yocto/gfx/ramp/r15.hpp"
#include "yocto/gfx/ramp/r16.hpp"
#include "yocto/gfx/ramp/r17.hpp"

#include "yocto/gfx/surface.hpp"
#include "yocto/gfx/image/jpg.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(ramp)
{
    const gfx::rgb_t   *maps[] = {
        gfx::ramp01,
        gfx::ramp02,
        gfx::ramp03,
        gfx::ramp04,
        gfx::ramp10,
        gfx::ramp14,
        gfx::ramp15,
        gfx::ramp16,
        gfx::ramp17

    };
    const gfx::unit_t   num = sizeof(maps)/sizeof(maps[0]);
    const gfx::unit_t   ns  = 1+num;
    const gfx::unit_t   rpm = 32;
    
    gfx::surface::pointer s( gfx::surface::create(gfx::pixel_format::ARGB32(), 512, rpm*ns) );
    
    s->ldz();
    
    gfx::JPG jpg;
    
    for(gfx::unit_t x=0;x<s->w;++x)
    {
        // default
        gfx::unit_t y=0;
        {
            const gfx::rgb_t   c = gfx::ramp::to_rgb<gfx::unit_t>(x,0,s->w-1);
            const gfx::pixel_t p = s->map_rgb(c);
            for(gfx::unit_t j=rpm;j>0;--j,++y)
            {
                s->put_pixel( (*s)[y][x], p );
            }
        }
        
        // ramps
        for(gfx::unit_t k=0;k<num;++k)
        {
            const gfx::rgb_t   c = gfx::ramp::to_rgb<gfx::unit_t>(x,maps[k],0,s->w-1);
            const gfx::pixel_t p = s->map_rgb(c);
            for(gfx::unit_t j=rpm;j>0;--j,++y)
            {
                s->put_pixel( (*s)[y][x], p );
            }
        }
    }
    
    
    jpg.save_surface("ramps.jpg", *s, "75");
    
}
YOCTO_UNIT_TEST_DONE()

