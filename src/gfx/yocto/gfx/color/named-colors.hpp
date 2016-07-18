#ifndef YOCTO_GFX_COLORS_NAMED_INCLUDED
#define YOCTO_GFX_COLORS_NAMED_INCLUDED 1

#include "yocto/gfx/color/data2rgba.hpp"
#include "yocto/gfx/color/named-colors-def.hpp"
#include "yocto/string.hpp"

namespace yocto
{
    namespace gfx
    {

        struct named_color
        {
            const char   *name;
            const uint8_t r;
            const uint8_t g;
            const uint8_t b;
            static const size_t count;
            static RGB fetch(const size_t indx) throw();
            static RGB query(const string &name);
            static RGB query(const char   *name);
        };

        template <typename T>
        class indx2rgba : public data2rgba
        {
        public:
            inline explicit indx2rgba(const size_t d=0) throw() : shift(d) {}
            inline virtual ~indx2rgba() throw() {}

            size_t shift;

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(indx2rgba);
            inline virtual RGBA get(const void *addr) throw()
            {
                const size_t indx = *(const T*)addr;
                if(indx<=0)
                {
                    return RGBA(0,0,0);
                }
                else
                {
                    return named_color::fetch(shift+indx);
                }
            }

        };
    }
}

#endif