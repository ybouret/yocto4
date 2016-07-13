#include "yocto/gfx/types.hpp"

namespace yocto
{
    namespace gfx
    {

        template <>
        uint8_t pixel<uint8_t>::opaque = 0xff;

        template <>
        float pixel<float>::opaque = 1.0f;

        template <>
        double pixel<double>::opaque = 1.0;



    }

}