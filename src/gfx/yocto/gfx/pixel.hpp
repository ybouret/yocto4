#ifndef YOCTO_GFX_PIXEL_INCLUDED
#define YOCTO_GFX_PIXEL_INCLUDED 1

#include "yocto/gfx/types.hpp"
#include "yocto/sequence/array.hpp"

namespace yocto
{
    namespace gfx
    {

        //! abstract operation for pixmaps: uint8_t, float, RGB...
        template <typename T>
        struct pixel
        {
            static const  T opaque;
            static bool     is_zero(const T) throw();
            static uint8_t  project(const T) throw();
            static T        invert(const T) throw();
            static T        average(const array<T> &ra) throw();
            static T        average(const T lhs, const T rhs) throw();
            static T        blend(const T bg, const T fg, const uint8_t alpha) throw();
            static float    to_float(const T) throw();
        };
    }
}

#endif

