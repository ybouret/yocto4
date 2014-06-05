#include "yocto/gfx/bitmap.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/core/offset.hpp"
#include <cstring>
#include "yocto/exceptions.hpp"
#include <iostream>
#include "yocto/code/bswap.hpp"

namespace yocto
{
    namespace gfx
    {
        
        bitmap:: ~bitmap() throw()
        {
            switch (model)
            {
                case memory_allocated:
                    memory::kind<memory::global>::release(entry, allocated);
                    break;
                    
                case memory_from_user:
                    break;
                    
                case memory_is_shared:
                    assert(shared);
                    if(shared->liberate())
                        delete shared;
                    break;
            }
            YOCTO_HARD_RESET(bitmap, d, this);
        }
        
        namespace
        {
            static inline unit_t __check_bmp_d( unit_t depth )
            {
                switch(depth)
                {
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 8:
                    case 16:
                        return depth;
                        
                    default:
                        break;
                }
                throw imported::exception("bitmap::depth","unsupported %d", int(depth));
            }
            
            static inline unit_t __check_bmp_w( unit_t width )
            {
                if( width <= 0 )
                    throw imported::exception("bitmap::width", "invalid w=%d", int(width) );
                return width;
            }
            
            static inline unit_t __check_bmp_h( unit_t height )
            {
                if( height <= 0 )
                    throw imported::exception("bitmap::height", "invalid h=%d", int(height) );
                return height;
            }
            
            static inline
            void *peek1(const void *p, unit_t dx )
            {
                return ((uint8_t*)p)+dx;
            }
            
            static inline
            void *peek2(const void *p, unit_t dx )
            {
                return ((uint16_t*)p)+dx;
            }
            
            static inline
            void *peek3(const void *p, unit_t dx )
            {
                return ((uint8_t*)p)+(3*dx);
            }
            
            static inline
            void *peek4(const void *p, unit_t dx )
            {
                return ((uint32_t*)p)+dx;
            }
            
            static inline
            void *peek8(const void *p, unit_t dx )
            {
                return ((uint64_t*)p)+dx;
            }
            
            static inline
            void *peek16(const void *p, unit_t dx )
            {
                return ((uint64_t*)p)+(dx+dx);
            }
            
            
            bitmap::peek_proc __get_peek( const unit_t depth ) throw()
            {
                switch(depth)
                {
                    case 1:
                        return peek1;
                        
                    case 2:
                        return peek2;
                        
                    case 3:
                        return peek3;
                        
                    case 4:
                        return peek4;
                        
                    case 8:
                        return peek8;
                        
                    case 16:
                        return peek16;
                }
                return 0;
            }
            
            
            
            static inline
            void swap3(void *a,void *b) throw()
            {
                uint8_t *p = (uint8_t*)a;
                uint8_t *q = (uint8_t*)b;
                core::bswap<1>(&p[0], &q[0]);
                core::bswap<1>(&p[1], &q[1]);
                core::bswap<1>(&p[2], &q[2]);
            }
            
            
            
            
            bitmap::swap_proc __get_swap( const unit_t depth ) throw()
            {
                switch(depth)
                {
                    case 1:
                        return core::bswap<1>;
                        
                    case 2:
                        return core::bswap<2>;
                        
                    case 3:
                        return swap3;
                        
                    case 4:
                        return core::bswap<4>;
                        
                    case 8:
                        return core::bswap<8>;
                    case 16:
                        return core::bswap<16>;
                }
                return 0;
            }
            
        }
        
        bitmap:: bitmap(unit_t depth, unit_t width, unit_t height) :
        d( __check_bmp_d(depth)  ),
        w( __check_bmp_w(width)  ),
        h( __check_bmp_h(height) ),
        pitch(  w*d ),
        stride( pitch ),
        entry(0),
        peek( __get_peek(d) ),
        swap( __get_swap(d) ),
        model( memory_allocated )
        {
            try
            {
                allocated = pitch * h;
                entry     = memory::kind<memory::global>::acquire(allocated);
            }
            catch(...)
            {
                YOCTO_HARD_RESET(bitmap, d, this);
                throw;
            }
        }
        
        
        bool  bitmap:: owns(const rectangle &rect) const throw()
        {
            //std::cerr << "rect " << rect.x << "," << rect.y << " + " << rect.w << "," << rect.h << std::endl;
            //std::cerr << "this " << w << "," << h << std::endl;
            return !(rect.x<0||rect.y<0||rect.xout>w||rect.yout>h);
        }
        
        
        bitmap:: bitmap( const bitmap::pointer &bmp, const rectangle *rect) :
        d(bmp->d),
        w( rect ? __check_bmp_w(rect->w) : bmp->w ),
        h( rect ? __check_bmp_h(rect->h) : bmp->h ),
        pitch( w*d ),
        stride( bmp->stride ),
        entry(0),
        peek(bmp->peek),
        swap(bmp->swap),
        model(memory_is_shared)
        {
            try
            {
                if(rect)
                {
                    if(!bmp->owns(*rect))
                        throw imported::exception("bitmap(bitmap::pointer,rect)","invalid rectangle");
                    entry = bmp->get(rect->x,rect->y);
                }
                else
                {
                    entry = bmp->entry;
                }
                shared = (bitmap *)bmp.__get();
                shared->withhold();
            }
            catch(...)
            {
                YOCTO_HARD_RESET(bitmap, d, this);
                throw;
            }
        }
        
        bitmap:: bitmap( const bitmap  &bmp, const rectangle *rect) :
        d(bmp.d),
        w( rect ? __check_bmp_w(rect->w) : bmp.w ),
        h( rect ? __check_bmp_h(rect->h) : bmp.h ),
        pitch( w*d ),
        stride(pitch ),
        entry(0),
        peek(bmp.peek),
        swap(bmp.swap),
        model(memory_allocated)
        {
            try
            {
                if(rect && !bmp.owns(*rect) )
                {
                    throw imported::exception("bitmap(bitmap,rect)", "invalid rectable");
                }
                allocated = pitch * h;
                entry     = memory::kind<memory::global>::acquire(allocated);
                
                uint8_t       *tgt = (uint8_t*)entry;
                const uint8_t *src = (const uint8_t *)(bmp.entry);
                const unit_t   tgt_stride = stride;
                const unit_t   src_stride = bmp.stride;
                assert(pitch == bmp.pitch);
                for(unit_t j=0;j<h;++j)
                {
                    memcpy(tgt,src,pitch);
                    tgt += tgt_stride;
                    src += src_stride;
                }
            }
            catch(...)
            {
                YOCTO_HARD_RESET(bitmap, d, this);
                throw;
            }
            
        }
        
        bitmap:: bitmap( unit_t depth, unit_t width, unit_t height, void *data, unit_t data_stride) :
        d( __check_bmp_d(depth)  ),
        w( __check_bmp_w(width)  ),
        h( __check_bmp_h(height) ),
        pitch( w*d ),
        stride(data_stride),
        entry(0),
        peek(__get_peek(depth)),
        swap(__get_swap(depth)),
        model(memory_from_user)
        {
            if(data_stride<pitch)
            {
                YOCTO_HARD_RESET(bitmap, d, this);
                throw imported::exception("bitmap(user_data)","data stride is too small");
            }
        }
        
        void * bitmap:: get_line(unit_t y) const throw()
        {
            return ( (uint8_t *) entry ) + y*stride;
        }
        
        void * bitmap:: get(unit_t x, unit_t y) const throw()
        {
            assert(peek);
            return peek( ( (uint8_t *) entry ) + y*stride, x);
        }
        
        void  bitmap:: flip_horz() throw()
        {
            uint8_t     *line = (uint8_t *)entry;
            const unit_t half = w/2;
            for(unit_t j=0;j<h;++j,line += stride)
            {
                uint8_t *p = line;
                uint8_t *q = line+pitch;
                for(unit_t i=0;i<half;++i)
                {
                    q -= d;
                    swap(p,q);
                    p += d;
                }
            }
        }
        
        void bitmap:: flip_vert() throw()
        {
            const unit_t half = h/2;
            uint8_t     *p    = (uint8_t *)entry;
            uint8_t     *q    = ((uint8_t *)entry) + h*stride;
            for(unit_t j=0;j<half;++j)
            {
                q -= stride;
                uint8_t *src = q;
                uint8_t *tgt = p;
                for(unit_t i=w;i>0;--i,++src,++tgt)
                {
                    cswap(*src,*tgt);
                }
                p += stride;
            }
        }
        
        void bitmap:: check_depths(const char *lhs_name,
                                   const int   lhs,
                                   const char *rhs_name,
                                   const int   rhs)
        {
            assert(lhs_name);
            assert(rhs_name);
            if(lhs != rhs )
                throw imported::exception("mismatch bitmap depths", "%s=%d != %s=%d", lhs_name, lhs, rhs_name, rhs);
        }
        
    }
}