#include "yocto/gfx/metrics.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace gfx
    {
        
        metrics:: ~metrics() throw()
        {
        }
        
        namespace
        {
            static inline size_t __check_depth(size_t bytes_per_pixel )
            {
                switch(bytes_per_pixel)
                {
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                        break;
                    default:
                        throw exception("invalid bytes per pixel=%u", unsigned(bytes_per_pixel));
                }
                return bytes_per_pixel;
            }
            
            static inline pixel_t __get1(const void *p)
            {
                return *(uint8_t*)p;
            }
            
            static inline void __put1(void *p, pixel_t c )
            {
                *(uint8_t*)p = uint8_t(c);
            }
            
            static inline pixel_t __get2(const void *p)
            {
                return *(uint16_t*)p;
            }
            
            static inline void __put2(void *p, pixel_t c )
            {
                *(uint16_t*)p = uint16_t(c);
            }
            
            
            static inline pixel_t __get4(const void *p)
            {
                return *(uint32_t*)p;
            }
            
            static inline void __put4(void *p, pixel_t c )
            {
                *(uint32_t*)p = uint32_t(c);
            }

            static inline pixel_t __get3(const void *p)
            {
                const uint8_t *q = (uint8_t *)p;
                return (pixel_t(q[2]) << 16) | (pixel_t(q[1])<<8) | pixel_t(q[0]);
            }
            
            static inline void __put3(void *p, pixel_t c )
            {
                uint8_t *q = (uint8_t*)p;
                q[0] = uint8_t( c     &0xff);
                q[1] = uint8_t((c>> 8)&0xff);
                q[2] = uint8_t((c>>16)&0xff);
            }

            
        }
        
        metrics:: metrics( size_t bytes_per_pixel ) :
        depth( __check_depth(bytes_per_pixel) ),
        get_pixel(0),
        put_pixel(0)
        {
            switch( depth )
            {
                case 1:
                    (get_pixel_proc&)get_pixel = __get1;
                    (put_pixel_proc&)put_pixel = __put1;
                    break;
                    
                case 2:
                    (get_pixel_proc&)get_pixel = __get2;
                    (put_pixel_proc&)put_pixel = __put2;
                    break;
                    
                    
                case 3:
                    (get_pixel_proc&)get_pixel = __get3;
                    (put_pixel_proc&)put_pixel = __put3;
                    break;
                    
                case 4:
                    (get_pixel_proc&)get_pixel = __get4;
                    (put_pixel_proc&)put_pixel = __put4;
                    break;
            }
        }
        
        metrics:: metrics(const metrics &other) throw() :
        depth(other.depth),
        get_pixel(other.get_pixel),
        put_pixel(other.put_pixel)
        {
            
        }

        
    }
}
