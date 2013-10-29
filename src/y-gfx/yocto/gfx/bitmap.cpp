#include "yocto/gfx/bitmap.hpp"
#include "yocto/exception.hpp"
#include "yocto/memory/global.hpp"

#include <cstring>
#include <iostream>

namespace yocto
{
    namespace gfx
    {
        
        bitmap * bitmap:: create(size_t Depth, unit_t W, unit_t H)
        {
            return new bitmap(Depth,W,H);
        }
        
        bitmap * bitmap::create_shared( const bitmap::pointer &bmp, const region *rect)
        {
            if(rect)
                return new bitmap(bmp,*rect);
            else
            {
                const region full(0,0,bmp->w,bmp->h);
                return new bitmap(bmp,full);
            }
        }
        
        
        bitmap * bitmap:: duplicate( const bitmap::pointer &bmp, const region *rect)
        {
            const region full(0,0,bmp->w,bmp->h);
            const region *area = rect ?  rect : &full;
            bitmap *dst = new bitmap(bmp->depth,area->w,area->h);
            
            uint8_t       *tgt = (uint8_t *)(dst->entry);
            const uint8_t *src = (uint8_t *)(bmp->get(area->x, area->y));
            const size_t   len = dst->pitch;
            for(unit_t j=area->h;j>0;--j)
            {
                memcpy(tgt,src,len);
                tgt += dst->stride;
                src += bmp->stride;
            }
            return dst;
        }

        
        bitmap:: ~bitmap() throw()
        {
            assert(entry);
            switch(type)
            {
                case in_memory:
                    assert(allocated>0);
                    assert(0==shared);
                    memory::kind<memory::global>::release(entry, (size_t&)allocated );
                    break;
                    
                case from_data:
                    // do nothing
                    assert(0==allocated);
                    assert(0==shared);
                    break;
                    
                case is_shared:
                    assert(0==allocated);
                    assert(0!=shared);
                    if(shared->liberate())
                    {
                        delete shared;
                    }
                    break;
            }
            memset( (void*)&w, 0, 1);
        }
        
        
        static inline
        unit_t __check_bitmap( unit_t Value, const char *Name )
        {
            if(Value<=0)
                throw exception("Negative Bitmap %s",Name);
            return Value;
        }
        
        static inline
        void * __move1(void *addr, unit_t n) throw()
        {
            return static_cast<uint8_t *>(addr)+n;
        }
        static inline
        void * __move2(void *addr, unit_t n) throw()
        {
            return static_cast<uint16_t *>(addr)+n;
        }
        static inline
        void * __move3(void *addr, unit_t n) throw()
        {
            return static_cast<uint8_t *>(addr)+(3*n);
        }
        static inline
        void * __move4(void *addr, unit_t n) throw()
        {
            return static_cast<uint32_t *>(addr)+n;
        }
        
        static inline
        void * __move8(void *addr, unit_t n) throw()
        {
            return static_cast<uint64_t *>(addr)+n;
        }
        
        static inline
        void * __move16(void *addr, unit_t n) throw()
        {
            return static_cast<uint64_t *>(addr)+(n+n);
        }
        
        
        
        static bitmap::peek_proc __assign_peek(size_t depth)
        {
            switch(depth)
            {
                case  1: return __move1;
                case  2: return __move2;
                case  3: return __move3;
                case  4: return __move4;
                case  8: return __move8;
                case 16: return __move16;
            }
            throw exception("Unsupported depth=%u", unsigned(depth) );
        }
        
        bitmap:: bitmap( size_t Depth, unit_t W, unit_t H ) :
        type( in_memory ),
        depth( __check_bitmap(Depth,"Depth" ) ),
        w(     __check_bitmap(W,    "Width" ) ),
        h(     __check_bitmap(H,    "Height") ),
        pitch( w*depth ),
        stride( pitch ),
        entry(0),
        peek( __assign_peek(depth) ),
        allocated( stride * h ),
        shared(0)
        {
            entry = memory::kind<memory::global>::acquire( (size_t&)allocated );
        }
        
        void bitmap:: ldz() throw()
        {
            uint8_t *p = (uint8_t*)entry;
            for(size_t i=h;i>0;--i)
            {
                memset(p,0,pitch);
                p += stride;
            }
        }
        
        bitmap:: bitmap( const bitmap::pointer &bmp, const region &rect ) :
        type( is_shared ),
        depth(bmp->depth),
        w( __check_bitmap(rect.w, "Shared Width")  ),
        h( __check_bitmap(rect.h, "Shared Height") ),
        pitch( w * depth ),
        stride( bmp->stride ),
        entry(0),
        peek(bmp->peek),
        allocated(0),
        shared(0)
        {
            if(rect.x<0||rect.xend>bmp->w) throw exception("Invalid Shared Bitmap x-offset");
            if(rect.y<0||rect.yend>bmp->h) throw exception("Invalid Shared Bitmap y-offset");
            
            shared = (bitmap *)( bmp.__get() );
            shared->withhold();
            
            uint8_t *p = static_cast<uint8_t*>(shared->entry);
            p += rect.y * stride;
            p += rect.x * depth;
            entry = p;
            
        }
        
        void * bitmap:: get_line(unit_t y) throw()
        {
            assert(y>=0);
            assert(y<h);
            assert(entry);
            return static_cast<uint8_t *>(entry)+(y*stride);
        }
     
        const void * bitmap:: get_line(unit_t y) const throw()
        {
            assert(y>=0);
            assert(y<h);
            assert(entry);
            return static_cast<uint8_t *>(entry)+(y*stride);
        }
        
        void * bitmap:: hmove(void *addr,      unit_t x) throw()
        {
            assert(addr);
            assert(peek);
            return peek(addr,x);
        }

        const void *bitmap:: hmove(const void *addr, unit_t x) const throw()
        {
            assert(addr);
            assert(peek);
            return peek((void*)addr,x);
        }
        
        void * bitmap:: get(unit_t x, unit_t y) throw()
        {
            assert(x>=0);
            assert(x<w);
            return hmove( get_line(y), x );
        }
        
        const void * bitmap:: get(unit_t x, unit_t y) const throw()
        {
            assert(x>=0);
            assert(x<w);
            return hmove( get_line(y), x );
        }
        
        
    }
}
