#include "yocto/gfx/pixel.hpp"

namespace yocto
{
    namespace gfx
    {

        template <>
        const uint8_t pixel<uint8_t>::opaque = 0xff;

        template <>
        const float pixel<float>::opaque = 1.0f;

        template <>
        const double pixel<double>::opaque = 1.0;


        // project

        template <>
        uint8_t pixel<uint8_t>::project(const uint8_t x) throw()
        {
            return x;
        }

        template <>
        uint8_t pixel<float>::project(const float x) throw()
        {
            return gist::float2byte(x);
        }

        // invert
        template <>
        uint8_t pixel<uint8_t>::invert(const uint8_t x) throw()
        {
            return 255-x;
        }

        template <>
        float pixel<float>::invert(const float x) throw()
        {
            return 1.0f-x;
        }
    }

}