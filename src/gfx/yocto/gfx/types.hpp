#ifndef YOCTO_GFX_TYPES_INCLUDED
#define YOCTO_GFX_TYPES_INCLUDED 1

#include "yocto/string.hpp"
#include "yocto/math/v2d.hpp"
#include <cmath>

namespace yocto
{
    namespace gfx
    {
        
        typedef uint32_t  pixel_t;
        
        struct conv
        {
            template <typename T> static inline
            string binary(T X)
            {
                string ans;
                for(unsigned i=0;i<sizeof(T);++i)
                {
                    ans += b2s( (X >> (((sizeof(T)-1)-i)<<3) ) & 0xff );
                }
                return ans;
            }
            
            static inline uint8_t to_byte( const float f ) throw()
            {
                return uint8_t( floorf(f*255.0f + 0.5f) );
            }

            static string b2s(uint8_t);
            
            static const float  unit_float[256];
            static const double unit_double[256];
            static float       greyscale_f(uint8_t r,uint8_t g,uint8_t b) throw();
            static uint8_t     greyscale(uint8_t r,uint8_t g,uint8_t b) throw();
        };
        
    }
}

#endif
