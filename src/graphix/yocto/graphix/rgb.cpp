#include "yocto/graphix/rgb.hpp"
#include <cmath>
#include <cfloat>

namespace yocto
{
    namespace graphix
    {

        template <>
        const uint8_t channel_info<uint8_t>::opaque = 0xff;

        template <>
        bool channel_info<uint8_t>::is_zero(const uint8_t c) throw()
        {
            return (c<=0);
        }

        template <>
        const float channel_info<float>::opaque = 1.0f;

        template <>
        bool channel_info<float>::is_zero(const float c) throw()
        {
            return fabsf(c)<=FLT_MIN;
        }

        
        
    }
    
}
