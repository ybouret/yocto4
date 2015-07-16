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

            //! address is a byte=>red channel
            static inline rgba_t from_byte_r(const void *addr, void *) throw()
            {
                assert(addr);
                const uint8_t u = *(const uint8_t *)addr;
                return rgba_t(u,0,0);
            }

            //! address is a byte=>red channel
            static inline rgba_t from_byte_g(const void *addr, void *) throw()
            {
                assert(addr);
                const uint8_t u = *(const uint8_t *)addr;
                return rgba_t(0,u,0);
            }


            //! address is a byte=>red channel
            static inline rgba_t from_byte_b(const void *addr, void *) throw()
            {
                assert(addr);
                const uint8_t u = *(const uint8_t *)addr;
                return rgba_t(0,0,u);
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
            static inline void to_rgba(void *addr, const rgba_t &C, void *) throw()
            {
                assert(addr);
                *(rgba_t *)addr = C;
            }

            static inline void to_rgb(void *addr, const rgba_t &C, void *) throw()
            {
                assert(addr);
                rgb_t &c = *(rgb_t *)addr;
                c.r = C.r;
                c.g = C.g;
                c.b = C.b;
            }

            static inline void to_gsf(void *addr, const rgba_t &C, void *) throw()
            {
                assert(addr);
                *(float *)addr = greyscale<float>(C.r, C.g, C.b);
            }
            
            static inline void to_bwf(void *addr, const rgba_t &C, void *) throw()
            {
                assert(addr);
                *(float *)addr = black_and_white<float>(C.r, C.g, C.b);
            }

        };

    }
}

#endif
