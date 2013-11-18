#include "yocto/gfx/metrics.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace gfx
    {
        
        metrics:: ~metrics() throw() {}
        
        
        metrics:: metrics( const metrics &other ) throw() :
        bytes(     other.bytes     ),
        get_pixel( other.get_pixel ),
        put_pixel( other.put_pixel )
        {
			assert(get_pixel);
			assert(put_pixel);
        }
        
        //======================================================================
        static inline
        pixel_t __get1(const void *p) throw()
        {
            assert(p);
            return *(uint8_t *)p;
        }
        
        static inline
        void __set1(void *p, pixel_t c) throw()
        {
            assert(p);
            *(uint8_t *)p = uint8_t(c);
        }
        
        //======================================================================
        static inline
        pixel_t __get2(const void *p) throw()
        {
            assert(p);
            return *(uint16_t *)p;
        }
        
        static inline
        void __set2(void *p, pixel_t c) throw()
        {
            assert(p);
            *(uint16_t *)p = uint16_t(c);
        }
        
        //======================================================================
        static inline
        pixel_t __get3(const void *p) throw()
        {
            assert(p);
            const uint8_t *q = (uint8_t*)p;
            return (pixel_t(q[0]) << 16) | (pixel_t(q[1])<<8) | pixel_t(q[2]);
        }
        
        static inline
        void __set3(void *p, pixel_t c) throw()
        {
            assert(p);
            uint8_t *q = (uint8_t*)p;
            q[0] = uint8_t((c>>16) & 0xff);
            q[1] = uint8_t((c>>8)  & 0xff);
            q[2] = uint8_t( c      & 0xff);
        }
        
        //======================================================================
        static inline
        pixel_t __get4(const void *p) throw()
        {
            assert(p);
            return *(uint32_t *)p;
        }
        
        static inline
        void __set4(void *p, pixel_t c) throw()
        {
            assert(p);
            *(uint32_t *)p = uint32_t(c);
        }
        
        
        metrics:: metrics(size_t Bytes) :
        bytes( Bytes ),
        get_pixel(0),
        put_pixel(0)
        {
            
            switch(bytes)
            {
                case 1:
                    get_pixel = __get1;
                    put_pixel = __set1;
                    break;
                    
                case 2:
                    get_pixel = __get2;
                    put_pixel = __set2;
                    break;
                    
                case 3:
                    get_pixel = __get3;
                    put_pixel = __set3;
                    break;
                    
                case 4:
                    get_pixel = __get4;
                    put_pixel = __set4;
                    break;
                    
                default:
                    throw exception("gfx::metrics(invalid bytes=%u)",unsigned(bytes));
            }
        }
        
        
        void metrics:: fmt(const uint32_t mask,
                           uint8_t       &bits,
                           uint8_t       &shift,
                           uint8_t       &loss,
                           const char    *field)
        {
            assert(field);
            if(mask<=0)
            {
                bits  = 0;
                shift = 0;
                loss  = 8;
            }
            else
            {
                uint32_t w         = 0x00;
                uint8_t  shift_max = 32;
                for(bits=1; bits<=8; ++bits )
                {
                    // build the pattern
                    w <<= 0x01;
                    w  |= 0x01;
                    
                    // find the shift
                    --shift_max;
                    for(shift=0;shift<=shift_max;++shift)
                    {
                        if(mask == (w<<shift) )
                        {
                            loss = 8-bits;
                            return;
                        }
                    }
                }
                throw exception("gfx::metrics::fmt(invalid '%s' mask=0x%08x)", field, unsigned(mask) );
            }
        }
        
    }
    
}
