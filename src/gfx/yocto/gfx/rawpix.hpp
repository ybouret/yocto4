#ifndef YOCTO_GFX_RAWPIX_INCLUDED
#define YOCTO_GFX_RAWPIX_INCLUDED 1

#include "yocto/gfx/pixmap.hpp"
#include "yocto/gfx/rgb.hpp"
#include "yocto/ios/ostream.hpp"

namespace yocto
{
    namespace gfx
    {
        
        typedef pixmap<rgb_t>  pixmap3;
        typedef pixmap<rgba_t> pixmap4;
        typedef pixmap<float>  pixmapf;
        typedef pixmap<double> pixmapd;

        typedef rgb_t        (*addr2rgb)(const void *);
        void save_ppm(ios::ostream &fp, const bitmap &bmp, addr2rgb proc,  const char *comment  = 0);
        void save_ppm(const string &fn, const bitmap &bmp, addr2rgb proc,  const char *comment  = 0);
        void save_ppm(const char   *fn, const bitmap &bmp, addr2rgb proc,  const char *comment  = 0);
        
        template <typename T>
        rgb_t to_rgb(const void *) throw();
        
        template <>
        inline rgb_t to_rgb<rgb_t>(const void *addr) throw()
        {
            assert(addr);
            return *(const rgb_t *)addr;
        }
        
        
        template <>
        inline rgb_t to_rgb<rgba_t>(const void *addr) throw()
        {
            assert(addr);
            const rgba_t C = *(const rgba_t *)addr;
            return rgb_t(C.r,C.g,C.b);
        }
        
        template <>
        inline rgb_t to_rgb<float>(const void *addr) throw()
        {
            assert(addr);
            const float   f = *(const float *)addr;
            const uint8_t u = conv::to_byte(f);
            return rgb_t(u,u,u);
        }
        
        
        template <>
        inline rgb_t to_rgb<double>(const void *addr) throw()
        {
            assert(addr);
            const double  f = *(const double *)addr;
            const uint8_t u = conv::to_byte(float(f));
            return rgb_t(u,u,u);
        }

        template <typename T>
        inline void save_ppm(ios::ostream &fp, const pixmap<T> &pxm, const char *comment = 0)
        {
            save_ppm(fp,pxm,to_rgb<T>,comment);
        }
        
        template <typename T>
        inline void save_ppm(const string &fn, const pixmap<T> &pxm, const char *comment = 0)
        {
            save_ppm(fn,pxm,to_rgb<T>,comment);
        }
        
        
        template <typename T>
        inline void save_ppm(const char *fn, const pixmap<T> &pxm, const char *comment = 0)
        {
            save_ppm(fn,pxm,to_rgb<T>,comment);
        }

    }
}

#endif
