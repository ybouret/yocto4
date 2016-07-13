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



    }

}