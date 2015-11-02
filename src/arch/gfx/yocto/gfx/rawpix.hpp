#ifndef YOCTO_GFX_RAWPIX_INCLUDED
#define YOCTO_GFX_RAWPIX_INCLUDED 1

#include "yocto/gfx/pixmap.hpp"
#include "yocto/gfx/rgb.hpp"
#include "yocto/ios/ostream.hpp"
#include "yocto/string.hpp"

namespace yocto
{
    namespace gfx
    {

        typedef pixmap<uint8_t> pixmap1;
        typedef pixmap<rgb_t>   pixmap3;
        typedef pixmap<rgba_t>  pixmap4;
        typedef pixmap<float>   pixmapf;
        typedef pixmap<double>  pixmapd;


        //! extraction rgb_t from an address
        typedef rgb_t (*addr2rgb)(const void *);
        void save_ppm(ios::ostream &fp, const bitmap &bmp, addr2rgb proc,  const char *comment  = 0);
        void save_ppm(const string &fn, const bitmap &bmp, addr2rgb proc,  const char *comment  = 0);
        void save_ppm(const char   *fn, const bitmap &bmp, addr2rgb proc,  const char *comment  = 0);

        //! template
        template <typename T>
        inline rgb_t to_ppm_rgb(const void *) throw();


        //! make greyscale...
        template <>
        inline rgb_t to_ppm_rgb<uint8_t>(const void *addr) throw()
        {
            assert(addr);
            const uint8_t u = *(const uint8_t *)addr;
            return rgb_t(u,u,u);
        }

        
        inline rgb_t to_ppm_r(const void *addr) throw()
        {
            assert(addr);
            const uint8_t u = *(const uint8_t *)addr;
            return rgb_t(u,0,0);
        }

        inline rgb_t to_ppm_g(const void *addr) throw()
        {
            assert(addr);
            const uint8_t u = *(const uint8_t *)addr;
            return rgb_t(0,u,0);
        }
        
        inline rgb_t to_ppm_b(const void *addr) throw()
        {
            assert(addr);
            const uint8_t u = *(const uint8_t *)addr;
            return rgb_t(0,0,u);
        }
        

        
        //! extract rgb, directly
        template <>
        inline rgb_t to_ppm_rgb<rgb_t>(const void *addr) throw()
        {
            assert(addr);
            return *(const rgb_t *)addr;
        }

        //! discard alpha channel
        template <>
        inline rgb_t to_ppm_rgb<rgba_t>(const void *addr) throw()
        {
            assert(addr);
            const rgba_t C = *(const rgba_t *)addr;
            return rgb_t(C.r,C.g,C.b);
        }

        //! assuming [0:1] range
        template <>
        inline rgb_t to_ppm_rgb<float>(const void *addr) throw()
        {
            const uint8_t u = to_byte<float>(*(const float *)addr);
            return rgb_t(u,u,u);
        }

        //! assuming [0:1] range
        template <>
        inline rgb_t to_ppm_rgb<double>(const void *addr) throw()
        {
            const uint8_t u = to_byte<double>(*(const double *)addr);
            return rgb_t(u,u,u);
        }


        template <typename OUTPUT,typename T>
        inline void save_ppm(OUTPUT &output, const pixmap<T> &pxm, const char *comment = 0)
        {
            save_ppm(output,pxm,to_ppm_rgb<T>,comment);
        }

    }
}

#endif

