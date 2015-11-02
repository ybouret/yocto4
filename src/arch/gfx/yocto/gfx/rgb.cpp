#include "yocto/gfx/rgb.hpp"

namespace yocto
{
    namespace gfx
    {
        
        template <>
        const uint8_t opaque<uint8_t>::value = 0xff;

        template <>
        uint8_t opaque<uint8_t>::scale(float x) throw()
        {
            return to_byte(x);
        }

        template <>
        const uint32_t opaque<uint32_t>::value = 0xffffffff;

        template <>
        uint32_t opaque<uint32_t>::scale(float x) throw()
        {
            static const float __the_max(value);
            return uint32_t( floorf(__the_max*x+0.5f ) );
        }

        template <>
        const float opaque<float>::value = 1.0f;

        template<>
        float opaque<float>::scale(float x) throw()
        {
            return x;
        }

        template<>
        double opaque<double>::scale(float x) throw()
        {
            return x;
        }

        template <>
        const double opaque<double>::value = 1.0;
        
        template <>
        const rgb_t opaque<rgb_t>::value = rgb_t(0xff,0xff,0xff);
        
        template <>
        const rgba_t opaque<rgba_t>::value = rgba_t(0xff,0xff,0xff,0xff);
        
        
    }
}

