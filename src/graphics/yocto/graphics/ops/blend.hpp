#ifndef YOCTO_GRAPHICS_OPS_BLEND_INCLUDED
#define YOCTO_GRAPHICS_OPS_BLEND_INCLUDED 1

#include "yocto/graphics/rgb.hpp"

namespace yocto
{
    namespace graphics
    {

        struct blend
        {
            template <typename COLOR>
            static COLOR mix( const COLOR &lhs, const COLOR &rhs, const typename COLOR::type alpha ) throw();
        };

        template <>
        inline RGB blend::mix<RGB>( const RGB &lhs, const RGB &rhs, const RGB::type alpha ) throw()
        {
            return RGB();
        }

    }
}

#endif
