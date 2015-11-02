#ifndef YOCTO_GFX_NAMED_COLORS_INCLUDED
#define YOCTO_GFX_NAMED_COLORS_INCLUDED 1

#include "yocto/graphics/rgb.hpp"
#include "yocto/string.hpp"

namespace yocto
{
    namespace graphix
    {
        struct named_color
        {
            const char   *name;
            const uint8_t r,g,b;
            
            static const named_color reg[];
            static const size_t      num;
            static RGB get(const char   *id);
            static RGB get(const string &id);
            static RGB fetch(size_t i) throw();
        };
        
        
    }
}


#endif
