#include "yocto/gfx/rawpix.hpp"
#include "yocto/ios/ostream.hpp"

namespace yocto
{
    namespace gfx
    {

        struct io_conv
        {
            //! loading a bitmap with depth 4
            static inline void put_rgba(void *addr, const rgba_t &C, const void *) throw()
            {
                assert(addr);
                *(rgba_t *)addr = C;
            }

            //! loading a bitmap with depth 3
            static inline void put_rgb(void *addr, const rgba_t &C, const void *) throw()
            {
                assert(addr);
                rgb_t &X = *(rgb_t*)addr;
                X.r = C.r;
                X.g = C.g;
                X.b = C.b;
            }

            //! loading a bitmap with depth 4, greyscale
            static inline void put_gsf(void *addr, const rgba_t &C, const void *) throw()
            {
                assert(addr);
                *(float *)addr = conv::greyscale_f(C.r, C.g, C.b);
            }

            //! loading a bitmap with depth 1, greyscale
            static inline void put_gsu(void *addr, const rgba_t &C, const void *) throw()
            {
                assert(addr);
                *(uint8_t *)addr = conv::greyscale(C.r, C.g, C.b);
            }

            //! saving a bitmap with depth 4
            static inline rgba_t get_rgba(const void *addr, const void *) throw()
            {
                assert(addr);
                return *(const rgba_t*)addr;
            }

            //! saving a bitmap with depth 4
            static inline rgba_t get_rgb(const void *addr, const void *) throw()
            {
                assert(addr);
                const rgb_t c = *(const rgb_t *)addr;
                return rgba_t(c.r,c.g,c.b);
            }

        };

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
