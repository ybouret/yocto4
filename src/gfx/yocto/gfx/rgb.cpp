#include "yocto/gfx/rgb.hpp"

namespace yocto
{
    namespace gfx
    {
        
        template <>
        const uint8_t opaque<uint8_t>::value = 0xff;
        
        template <>
        const uint32_t opaque<uint32_t>::value = 0xffffffff;
      
        template <>
        const float opaque<float>::value = 1.0f;

        template <>
        const double opaque<double>::value = 1.0;
        
        template <>
        const rgb_t opaque<rgb_t>::value = rgb_t(0xff,0xff,0xff);
        
        template <>
        const rgba_t opaque<rgba_t>::value = rgba_t(0xff,0xff,0xff,0xff);
        
        
    }
}