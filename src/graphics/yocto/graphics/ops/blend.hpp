#ifndef YOCTO_GRAPHICS_OPS_BLEND_INCLUDED
#define YOCTO_GRAPHICS_OPS_BLEND_INCLUDED 1

#include "yocto/graphics/rgb.hpp"

namespace yocto
{
    namespace graphics
    {

        struct blend
        {
            static
            inline RGB mix( const RGB &lhs, const RGB &rhs, const RGB::type alpha ) throw()
            {
                const int av = alpha;
                const int au = 255-av;
                const RGB::type *u = &lhs.r;
                const RGB::type *v = &rhs.r;
                return RGB(
                           (( int(u[0])*au + int(v[0])*av )/255),
                           (( int(u[1])*au + int(v[1])*av )/255),
                           (( int(u[2])*au + int(v[2])*av )/255)
                           );
            }

        };


    }
}

#endif
