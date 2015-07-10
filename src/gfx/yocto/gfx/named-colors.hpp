#ifndef YOCTO_GFX_NAMED_COLORS_INCLUDED
#define YOCTO_GFX_NAMED_COLORS_INCLUDED 1

#include "yocto/gfx/rgb.hpp"
#include "yocto/string.hpp"

namespace yocto
{
    namespace gfx
    {
        struct named_color
        {
            const char   *name;
            const uint8_t r,g,b;
            
            static const named_color reg[];
            static const size_t      num;
            static rgb_t get(const char   *id);
            static rgb_t get(const string &id);
            static rgb_t fetch(size_t i) throw();
        };
        
        
    }
}


#endif
