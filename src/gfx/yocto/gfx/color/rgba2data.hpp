#ifndef YOCTO_GFX_COLOR_RGBA2DATA_INCLUDED
#define YOCTO_GFX_COLOR_RGBA2DATA_INCLUDED 1

#include "yocto/gfx/color/rgb.hpp"

namespace yocto
{
    namespace gfx
    {

        class rgba2data : public object
        {
        public:
            virtual ~rgba2data() throw();

            void operator()(void *addr, const RGBA &c);

        protected:
            explicit rgba2data() throw();

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(rgba2data);
            virtual void put(void *addr,const RGBA &c) = 0;
        };

#define YOCTO_GRAPHIX_RGBA2DATA(CLASS,CODE)          \
class CLASS : public rgba2data {                     \
public:                                              \
inline explicit CLASS() throw() : rgba2data() {}     \
inline virtual ~CLASS() throw() {}                   \
private:                                             \
YOCTO_DISABLE_COPY_AND_ASSIGN(CLASS);                \
virtual void put(void *addr,const RGBA &C) { CODE; } \
}


        //! for pixmap4
        YOCTO_GRAPHIX_RGBA2DATA(put_rgba,*(RGBA *)addr=C);

        //! for pixmap3
        YOCTO_GRAPHIX_RGBA2DATA(put_rgb,new (addr) RGB(C.r,C.g,C.b));

        //! for pixmapf, greyscale
        YOCTO_GRAPHIX_RGBA2DATA(put_gsf,*(float *)addr =  gist::greyscalef(C.r, C.g, C.b) );

        //! for pixmap1, greyscale
        YOCTO_GRAPHIX_RGBA2DATA(put_gsu,*(uint8_t*)addr = gist::greyscale1(C.r, C.g, C.b) );

    }

}
#endif

