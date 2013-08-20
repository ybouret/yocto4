#ifndef YOCTO_GFX_TYPES_INCLUDED
#define YOCTO_GFX_TYPES_INCLUDED 1

#include "yocto/string.hpp"
#include <cmath>

namespace yocto
{
    
    namespace gfx
    {
        typedef uint32_t color_t;
        
        template <typename T>
        struct alpha_max;
        
        template <>
        struct alpha_max<uint8_t>
        {
            enum { value = 0xff };
        };
        
        
        template <>
        struct alpha_max<float>
        {
            enum { value = 1 };
        };
        
        
        struct conv
        {
            static string to_binary(const uint8_t  &b);
            static string to_binary(const uint32_t &b);
            
            static inline uint8_t f2b( float f ) throw()
            {
                return uint8_t( floorf(f*255.0f + 0.5f) );
            }
        };
        
    }
}

#endif
