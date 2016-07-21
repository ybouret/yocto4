#ifndef YOCTO_GFX_COLOR_DATA2RGBA_INCLUDED
#define YOCTO_GFX_COLOR_DATA2RGBA_INCLUDED 1

#include "yocto/gfx/color/yuv.hpp"

namespace yocto
{
    namespace gfx
    {

        //! create RGBA from data
        class data2rgba : public object
        {
        public:
            virtual ~data2rgba() throw();

            RGBA operator()(const void *addr);


        protected:
            explicit data2rgba() throw();

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(data2rgba);
            virtual RGBA get(const void *addr) = 0;
        };


#define YOCTO_GRAPHIX_DATA2RGBA(CLASS,CODE)          \
class CLASS : public data2rgba {                     \
public:                                              \
inline explicit CLASS() throw() : data2rgba() {}     \
inline virtual ~CLASS() throw() {}                   \
private:                                             \
YOCTO_DISABLE_COPY_AND_ASSIGN(CLASS);                \
virtual RGBA get(const void *addr) { CODE; } \
}

        //! for pixmap4
        YOCTO_GRAPHIX_DATA2RGBA(rgba2rgba,return *(const RGBA*)addr);

        //! for pixmap3
        YOCTO_GRAPHIX_DATA2RGBA(rgb2rgba, return RGBA(*(const RGB*)addr) );

        //! for pixmapf
        YOCTO_GRAPHIX_DATA2RGBA(float2rgba,
                                const uint8_t u=gist::float2byte(*(const float *)addr);
                                return RGBA(u,u,u)
                                );


        //! for pixmapu
        YOCTO_GRAPHIX_DATA2RGBA(byte2rgba,
                                const uint8_t u=*(const uint8_t *)addr;
                                return RGBA(u,u,u)
                                );


        //! for pixmap 1,red-scale
        YOCTO_GRAPHIX_DATA2RGBA(red2rgba,   return RGBA(*(const uint8_t*)addr,0,0,255) );

        //! for pixmap 1, green-scale
        YOCTO_GRAPHIX_DATA2RGBA(green2rgba, return RGBA(0,*(const uint8_t*)addr,0,255) );

        //! for pixmap 1, blue-scale
        YOCTO_GRAPHIX_DATA2RGBA(blue2rgba,  return RGBA(0,0,*(const uint8_t*)addr,255) );

        //! for complex to greyscale
        YOCTO_GRAPHIX_DATA2RGBA(cplx2rgba,
                                const cplx_t  c = *(const cplx_t *)addr;
                                const uint8_t u = gist::float2byte(c.mod());
                                return RGBA(u,u,u)
                                );

        //! for YUV
        YOCTO_GRAPHIX_DATA2RGBA(yuv2rgba,return YUV::toRGB(*(const YUV*)addr) );

    }

}

#endif
