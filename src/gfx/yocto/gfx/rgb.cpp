#include "yocto/gfx/rgb.hpp"

namespace yocto
{
    namespace gfx
    {
        
        template <>
        const uint8_t opaque<uint8_t>::value = 0xff;

        template <>
        uint8_t opaque<uint8_t>::scale(double x) throw()
        {
            return uint8_t( floor(255.0*x+0.5 ) );
        }

        template <>
        const uint32_t opaque<uint32_t>::value = 0xffffffff;

        template <>
        uint32_t opaque<uint32_t>::scale(double x) throw()
        {
            static const double __max(value);
            return uint32_t( floor(__max*x+0.5 ) );
        }

        template <>
        const float opaque<float>::value = 1.0f;

        template<>
        float opaque<float>::scale(double x) throw()
        {
            return float(x);
        }

        template<>
        double opaque<double>::scale(double x) throw()
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

