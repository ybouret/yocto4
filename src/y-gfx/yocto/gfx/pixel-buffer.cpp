#include "yocto/gfx/pixel-buffer.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/exception.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace gfx
    {
        size_t pixbuf:: length() const throw() { return bytes; }
        
        pixbuf:: ~pixbuf() throw()
        {
            if(allocated)
            {
                memory::kind<memory::global>::release(data, items);
            }
            else
            {
                data  = 0;
                items = 0;
            }
        }
        
        static inline
        size_t __check_bytes_per_pixel( size_t BPP )
        {
            if(BPP<1||BPP>4)
                throw exception("invalid pixbuf bytes_per_pixel=%u", unsigned(BPP) );
            return BPP;
        }
        
        static inline
        void * peek8(void *addr, unit_t x, unit_t y, const size_t *param) throw()
        {
            assert(addr);
            assert(param);
            const unit_t w = unit_t(param[0]); assert(param[1]==param[0]);
            uint8_t *p = static_cast<uint8_t*>(addr);
            return &p[y*w+x];
        }
        
        
        static inline
        void *peek16(void *addr, unit_t x, unit_t y, const size_t *param) throw()
        {
            assert(addr);
            assert(param);
            const unit_t w = unit_t(param[0]); assert(param[1]==2*param[0]);
            uint16_t *p = static_cast<uint16_t*>(addr);
            return &p[y*w+x];
        }
        
        
        static inline
        void *peek32(void *addr, unit_t x, unit_t y, const size_t *param) throw()
        {
            assert(addr);
            assert(param);
            const unit_t w = unit_t(param[0]); assert(param[1]==4*param[0]);
            uint32_t *p = static_cast<uint32_t*>(addr);
            return &p[y*w+x];
        }
        
        static inline
        void *peek24( void *addr, unit_t x, unit_t y, const size_t *param) throw()
        {
            assert(addr);
            assert(param);
            const unit_t s = unit_t(param[1]); assert(param[1]==3*param[0]);
            uint8_t *p = static_cast<uint8_t*>(addr);
            return &p[s*y+3*x];
        }
        
        static inline
        pixbuf::peek_proc __get_peek( size_t BPP ) throw()
        {
            assert(BPP>=1);
            assert(BPP<=4);
            switch( BPP )
            {
                case 1:
                    return peek8;
                    
                case 2:
                    return peek16;
                    
                case 3:
                    return peek24;
                    
                case 4:
                    return peek32;
            }
            return 0;
        }
        
        
        pixbuf:: pixbuf(size_t BPP, size_t width, size_t height) :
        bytes_per_pixel(__check_bytes_per_pixel(BPP)),
        w( max_of<size_t>(1,width)  ),
        stride( w * bytes_per_pixel ),
        h( max_of<size_t>(1,height) ),
        bytes( stride * h ),
        items( bytes ),
        data( memory::kind<memory::global>::acquire(items) ),
        peek( __get_peek(bytes_per_pixel) ),
        allocated(true)
        {
            
        }
        
        
        pixbuf:: pixbuf( size_t BPP, size_t width, size_t height, void *user_data) :
        bytes_per_pixel(__check_bytes_per_pixel(BPP)),
        w( max_of<size_t>(1,width)  ),
        stride( w * bytes_per_pixel ),
        h( max_of<size_t>(1,height) ),
        bytes( stride * h ),
        items( bytes ),
        data( user_data ),
        peek( __get_peek(bytes_per_pixel) ),
        allocated(false)
        {
            assert(user_data);
        }

        
        const void *pixbuf:: get_address() const throw() { return (void*)data; }
        
        void * pixbuf::get(unit_t x, unit_t y) throw()
        {
            assert(peek);
            return peek(data, x, y, &w);
        }
        
        const void * pixbuf::get(unit_t x, unit_t y) const throw()
        {
            assert(peek);
            return peek((void*)data, x, y, &w);
        }
        
    }
    
}

