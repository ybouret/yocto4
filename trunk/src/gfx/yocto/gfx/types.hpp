#ifndef YOCTO_GFX_TYPES_INCLUDED
#define YOCTO_GFX_TYPES_INCLUDED 1

#include "yocto/string.hpp"
#include "yocto/math/v2d.hpp"
#include <cmath>

namespace yocto
{
    
    namespace gfx
    {
        typedef ptrdiff_t unit_t;
        typedef uint32_t  pixel_t;
        
        struct conv
        {
            static string to_binary(const uint8_t  &b);
            static string to_binary(const uint32_t &b);
            
            static inline uint8_t to_byte( const float f ) throw()
            {
                return uint8_t( floorf(f*255.0f + 0.5f) );
            }
            
            static const float unit_float[256];
            static uint8_t greyscale( const uint8_t r, const uint8_t g, const uint8_t b) throw();
        };
        
        
    }
}

#endif

