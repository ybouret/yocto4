#ifndef YOCTO_GRAPHIX_DATA2RGBA_INCLUDED
#define YOCTO_GRAPHIX_DATA2RGBA_INCLUDED 1

#include "yocto/graphics/named-colors.hpp"

namespace yocto
{
    namespace graphics
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
        YOCTO_GRAPHIX_DATA2RGBA(get_rgba,return *(const RGBA*)addr);

        //! for pixmap3
        YOCTO_GRAPHIX_DATA2RGBA(get_rgb, return RGBA(*(const RGB*)addr) );

        //! for pixmapf
        YOCTO_GRAPHIX_DATA2RGBA(get_gsf,
                                const uint8_t u=gist::float2byte(*(const float *)addr);
                                return RGBA(u,u,u)
                                );

        //! for pixmapf
        class get_rampf : public data2rgba
        {
        public:
            float vmin;
            float vmax;
            inline explicit get_rampf() throw() : data2rgba(), vmin(0), vmax(1) {}
            inline virtual ~get_rampf() throw() {}

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(get_rampf);
            virtual RGBA get(const void *addr)
            {
                return default_ramp<RGBA>( *(const float *)addr,vmin,vmax);
            }
        };

        //! for pixmapu
        YOCTO_GRAPHIX_DATA2RGBA(get_gsu,
                                const uint8_t u=*(const uint8_t *)addr;
                                return RGBA(u,u,u)
                                );

        //! for pixmap int/unsigned
        template <typename T>
        YOCTO_GRAPHIX_DATA2RGBA(get_named_color,
                                const  T indx = *(const T *)addr;
                                return (indx!=0) ? named_color::fetch(indx) : RGB(0,0,0)
                                );

        //! for pixmap 1,red-scale
        YOCTO_GRAPHIX_DATA2RGBA(get_red,   return RGBA(*(const uint8_t*)addr,0,0,255) );

        //! for pixmap 1, green-scale
        YOCTO_GRAPHIX_DATA2RGBA(get_green, return RGBA(0,*(const uint8_t*)addr,0,255) );

        //! for pixmap 1, blue-scale
        YOCTO_GRAPHIX_DATA2RGBA(get_blue,  return RGBA(0,0,*(const uint8_t*)addr,255) );

        
    }

}

#endif

