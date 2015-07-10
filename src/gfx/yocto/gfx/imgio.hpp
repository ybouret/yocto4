#ifndef YOCTO_GFX_IMGIO_INCLUDED
#define YOCTO_GFX_IMGIO_INCLUDED 1

#include "yocto/gfx/rgb.hpp"

namespace yocto
{
    namespace gfx
    {

        struct get_rgba
        {
            //! address is a rgba_t
            static inline rgba_t from_rgba(const void *addr,void *) throw()
            {
                assert(addr);
                return *(const rgba_t *)addr;
            }

            //! address is a rgb_t
            static inline rgba_t from_rgb(const void *addr,void *) throw()
            {
                assert(addr);
                return rgba_t(*(const rgb_t *)addr);
            }

            //! address is a byte
            static inline rgba_t from_byte(const void *addr,void *) throw()
            {
                assert(addr);
                const uint8_t u = *(const uint8_t *)addr;
                return rgba_t(u,u,u);
            }

            //! address is a float greyscale
            static inline rgba_t from_gsf(const void *addr, void *) throw()
            {
                assert(addr);
                const uint8_t u = to_byte<float>( *(const float *)addr );
                return rgba_t(u,u,u);
            }

            //! address is a float greyscale, convert to ramp
            static inline rgba_t from_rampf(const void *addr, void *) throw()
            {
                assert(addr);
                return rgba_t::make_ramp( *(const float *)addr );
            }

        };


        struct put_rgba
        {

        };

    }
}

#endif
