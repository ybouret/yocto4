#include "yocto/utest/run.hpp"
#include "yocto/gfx/surface.hpp"
#include "yocto/gfx/named-rgb.hpp"

using namespace yocto;

namespace  {
    
    static inline
    void fill_surface( gfx::surface &s )
    {
        
        std::cerr << "Surface " << s.w << "x" << s.h << ", depth=" << s.depth << ", bpp=" << s.bpp << std::endl;
        
        const gfx::rgb_t col[4] =
        {
            gfx::get_named_rgb("blue"),
            gfx::get_named_rgb("red"),
            gfx::get_named_rgb("green"),
            gfx::get_named_rgb("black")
        };
        
        
        const float r00 = col[0].r/255.0f;
        const float g00 = col[0].g/255.0f;
        const float b00 = col[0].b/255.0f;
        
        const float r01 = col[1].r/255.0f;
        const float g01 = col[1].g/255.0f;
        const float b01 = col[1].b/255.0f;
        
        const float r11 = col[2].r/255.0f;
        const float g11 = col[2].g/255.0f;
        const float b11 = col[2].b/255.0f;
        
        const float r10 = col[3].r/255.0f;
        const float g10 = col[3].g/255.0f;
        const float b10 = col[3].b/255.0f;
        
        for( gfx::unit_t y=0;y<s.h;++y)
        {
            const float wy = float(y)/s.h;
            const float my = 1.0f - wy;
            for( gfx::unit_t x=0;x<s.w;++x)
            {
                const float wx = float(x)/s.w;
                const float mx = 1.0f-wx;
                const float w00 = mx*my;
                const float w11 = wx*wy;
                const float w01 = mx*wy;
                const float w10 = wx*my;
                const uint8_t alpha = gfx::conv::to_byte( 0.5f + 0.5f*float(x*y)/(s.h*s.w));
                const uint8_t r = gfx::conv::to_byte( w00 * r00 + w11 * r11 + w01 * r01 + w10 * r10 );
                const uint8_t g = gfx::conv::to_byte( w00 * g00 + w11 * g11 + w01 * g01 + w10 * g10 );
                const uint8_t b = gfx::conv::to_byte( w00 * b00 + w11 * b11 + w01 * b01 + w10 * b10 );
                s.put_pixel( s[y][x], s.map_rgba(r, g, b, alpha) );
                if(x==0&&y==0)
                {
                    std::cerr << "Top Left=" << int(r) << " " << int(g) << " " << int(b) << std::endl;
                }
            }
        }
    }
    
}

YOCTO_UNIT_TEST_IMPL(surface)
{
	singleton_verbosity = true;
    std::cerr << "sizeof(gfx::surface)=" << sizeof(gfx::surface) << std::endl;
    gfx::surface::pointer s1 = gfx::surface::create( gfx::pixel_format::RGB24(), 100, 50);

    fill_surface(*s1);
    
    gfx::surface::pointer s2 = gfx::surface::create( gfx::pixel_format::ARGB32(), 100, 50);
    fill_surface(*s2);
    
    gfx::surface::pointer s3 = gfx::surface::create( gfx::pixel_format::ARGB16(), 100, 50);
	fill_surface(*s3);
    
}
YOCTO_UNIT_TEST_DONE()


YOCTO_UNIT_TEST_IMPL(save)
{
    gfx::surface::pointer s = gfx::surface::create( gfx::pixel_format::ARGB32(), 256, 128);
    fill_surface(*s);
    
    gfx::addr2rgba proc( &*s, & gfx::surface::to_rgba);
    
    s->save("image.tga",    s->TGA,    proc);
    s->save("image_a.tga",  s->TGA_A,  proc);
    s->save("image_z.tga",  s->TGA_Z,  proc);
    s->save("image_za.tga", s->TGA_ZA, proc);
    s->save("image.ppm",    s->PPM,    proc);
    s->save("image.tiff",   s->TIFF,   proc);
    s->save("image.eps",    s->EPS,    proc);
    s->save("image_bw.eps", s->EPS_BW, proc);
    s->save("image.bmp",    s->BMP,    proc);
    s->save("image.rgb",    s->RGB,    proc);
    s->save("image.raw",    s->RAW,    proc);
    s->save("image_bw.raw", s->RAW_BW, proc);
    
}
YOCTO_UNIT_TEST_DONE()
