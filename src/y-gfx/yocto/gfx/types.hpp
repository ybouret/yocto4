#ifndef YOCTO_GFX_TYPES_INCLUDED
#define YOCTO_GFX_TYPES_INCLUDED 1

#include "yocto/string.hpp"
#include <cmath>

namespace yocto
{
    
    namespace gfx
    {
        typedef ptrdiff_t unit_t;
        
        struct conv
        {
            static string to_binary(const uint8_t  &b);
            static string to_binary(const uint32_t &b);
            
            static inline uint8_t to_byte( const float f ) throw()
            {
                return uint8_t( floorf(f*255.0f + 0.5f) );
            }
            
        };
        
        
    }
}

#endif

