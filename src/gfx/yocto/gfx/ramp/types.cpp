#include "yocto/gfx/ramp/types.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace gfx
    {
        rgb_t ramp:: get(float v) throw()
        {
            struct { float r,g,b; } c = {1.0f,1.0f,1.0f};
            v = clamp<float>(0,v,1);
            
            
            if (v<0.25f)
            {
                c.r = 0.0f;
                c.g = 4.0f * v;
            }
            else if (v<0.5f)
            {
                c.r = 0.0f;
                c.b = 1.0f + 4.0f * (0.25f - v) ;
            }
            else if (v<0.75f)
            {
                c.r = 4.0f * (v - 0.5f);
                c.b = 0.0f;
            } else {
                c.g = 1.0f + 4.0f * (0.75f  - v);
                c.b = 0.0f;
            }
            
            return rgb_t( gfx::conv::to_byte(c.r), gfx::conv::to_byte(c.g), gfx::conv::to_byte(c.b) );
        }
        
        rgb_t ramp:: get(float v, const rgb_t *colors) throw()
        {
            assert(colors);
            return colors[ gfx::conv::to_byte(clamp<float>(0,v,1)) ];
        }
        
    }
}
