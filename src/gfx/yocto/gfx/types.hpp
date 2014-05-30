#ifndef YOCTO_GFX_TYPES_INCLUDED
#define YOCTO_GFX_TYPES_INCLUDED 1

#include "yocto/string.hpp"
#include "yocto/math/v2d.hpp"

namespace yocto
{
    namespace gfx
    {
        
        typedef ptrdiff_t unit_t;
        
        struct __conv
        {
            template <typename T> static inline
            string binary(T X)
            {
                string ans;
                for(unsigned i=0;i<sizeof(T);++i)
                {
                    ans += bs2( (X >> (((sizeof(T)-1)-i)<<3) ) & 0xff );
                }
                return ans;
            }
            static string b2s(uint8_t);
        };
        
    }
}

#endif
