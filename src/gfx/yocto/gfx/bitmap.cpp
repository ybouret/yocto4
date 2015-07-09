#include "yocto/gfx/bitmap.hpp"
#include "yocto/exception.hpp"
#include "yocto/core/offset.hpp"
#include <cstring>
#include "yocto/code/bswap.hpp"

namespace yocto
{
    namespace gfx
    {

        bitmap:: ~bitmap() throw()
        {
            switch (model)
            {
                case memory_is_global:
                    memory::kind<memory::global>::release(entry,allocated);
                    break;

                case memory_is_shared:
                    assert(shared);
                    assert(shared->refcount()>0);
                    if(shared->liberate())
                    {
                        delete shared;
                    }
                    break;

                case memory_from_user:
                    break;
            }
            YOCTO_HARD_RESET(bitmap, d, this);
        }


        unit_t bitmap:: check_depth(const unit_t D)
        {
            switch (D) {
                case 1:
                case 2:
                case 3:
                case 4:
                case 8:
                case 16:
                    return D;

                default:
                    break;
            }
            throw exception("bitmap(invalid depth=%d)", int(D));
        }

        unit_t bitmap:: check_width(const unit_t W)
        {
            if(W<=0) throw exception("bitmap(invalid width=%d)", int(W));
            return W;
        }

        unit_t bitmap:: check_height(const unit_t H)
        {
            if(H<=0) throw exception("bitmap(invalid height=%d)", int(H));
            return H;
        }

        namespace
        {
            static inline
            void * peek1(void *addr,const unit_t dx) throw()
            {
                return static_cast<uint8_t*>(addr)+dx;
            }

            static inline
            void * peek2(void *addr,const unit_t dx) throw()
            {
                return static_cast<uint16_t*>(addr)+dx;
            }

            static inline
            void * peek3(void *addr,const unit_t dx) throw()
            {
                return static_cast<uint8_t*>(addr)+3*dx;
            }

            static inline
            void * peek4(void *addr,const unit_t dx) throw()
            {
                return static_cast<uint32_t*>(addr)+dx;
            }

            static inline
            void * peek8(void *addr,const unit_t dx) throw()
            {
                return static_cast<uint64_t*>(addr)+dx;
            }

            static inline
            void * peek16(void *addr,const unit_t dx) throw()
            {
                return static_cast<uint64_t*>(addr)+(dx+dx);
            }

            bitmap::peek_proc fetch_peek( const unit_t d )
            {
                switch(d)
                {
                    case 1:  return peek1;
                    case 2:  return peek2;
                    case 3:  return peek3;
                    case 4:  return peek4;
                    case 8:  return peek8;
                    case 16: return peek16;

                    default:
                        break;
                }
                throw exception("bitmap: no peek proc for depth=%d", int(d));
            }
        }


        namespace
        {

            bitmap::swap_proc fetch_swap( const unit_t d )
            {
                switch(d)
                {
                    case 1:  return core::bswap<1>;
                    case 2:  return core::bswap<2>;
                    case 3:  return core::bswap<3>;
                    case 4:  return core::bswap<4>;
                    case 8:  return core::bswap<8>;
                    case 16: return core::bswap<16>;
                    default:
                        break;
                }
                throw exception("bitmap: no swap proc for depth=%d", int(d));
            }

        }


        bitmap:: bitmap(const unit_t D,
                        const unit_t W,
                        const unit_t H) :
        d( check_depth(D)  ),
        w( check_width(W)  ),
        h( check_height(H) ),
        pitch(w*d),
        stride(pitch),
        entry(0),
        peek(fetch_peek(d)),
        model(memory_is_global),
        swap(fetch_swap(d))
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


        bitmap::bitmap(const unit_t D,
                       const unit_t W,
                       const unit_t H,
                       void        *data,
                       const unit_t S) :
        d( check_depth(D)  ),
        w( check_width(W)  ),
        h( check_height(H) ),
        pitch(w*d),
        stride(S),
        entry(data),
        peek(fetch_peek(d)),
        model(memory_from_user),
        swap(fetch_swap(d))
        {
            try
            {
                if(stride<pitch) throw exception("bitmap: invalid user stride!");
                if(!entry)       throw exception("bitmap: null user data!");
            }
            catch(...)
            {
                YOCTO_HARD_RESET(bitmap, d, this);
                throw;
            }
        }

        void bitmap:: ldz() throw()
        {
            uint8_t *line = (uint8_t *)entry;
            for(unit_t j=0;j<h;++j,line+=stride)
            {
                memset(line,0,pitch);
            }

        }


        void bitmap:: flip_vert() throw()
        {
            uint8_t *lo = (uint8_t*)get_line(0);
            uint8_t *up = (uint8_t*)get_line(h);
            for(unit_t j=h/2;j>0;--j,lo+=stride )
            {
                up -= stride;
                uint8_t *p = lo;
                uint8_t *q = up;
                for(unit_t i=w;i>0;--i,p+=d,q+=d)
                {
                    swap(p,q);
                }
            }
            
        }
        
        
    }
}

