#ifndef YOCTO_GFX_TYPES_INCLUDED
#define YOCTO_GFX_TYPES_INCLUDED 1

#include "yocto/string.hpp"

namespace yocto
{
    
    namespace gfx
    {
        
        struct output_bits
        {
            static string binary(const uint8_t &b);
        };
        
    }
}

#endif
