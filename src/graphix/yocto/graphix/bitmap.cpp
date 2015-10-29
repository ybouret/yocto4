#include "yocto/graphix/bitmap.hpp"
#include "yocto/exception.hpp"
#include <cstdlib>

namespace yocto
{
    namespace graphix
    {

        bitmap:: ~bitmap() throw()
        {
            switch (model) {
                case memory_is_global:
                    assert(entry);
                    assert(allocated);
                    memory::kind<memory::global>::release(entry,allocated);
                    break;

                case memory_is_shared:
                    assert(shared);
                    if(shared->liberate())
                    {
                        delete shared;
                    }
                    break;

                case memory_from_user:
                    // do nothing
                    break;
            }
        }

        unit_t bitmap:: check_d(unit_t D)
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
            throw exception("bitmap: invalid depth=%ld",long(D));
        }

        unit_t bitmap:: check_w(unit_t W)
        {
            if(W<=0) throw exception("bitmap: invalid width");
            return W;
        }

        unit_t bitmap:: check_h(unit_t H)
        {
            if(H<=0) throw exception("bitmap: invalid height");
            return H;
        }

        static inline void * xshift1(void *start, unit_t dx)
        {
            return static_cast<uint8_t*>(start)+dx;
        }


        static inline void * xshift2(void *start, unit_t dx)
        {
            return static_cast<uint16_t*>(start)+dx;
        }

        static inline void * xshift3(void *start, unit_t dx)
        {
            return static_cast<uint8_t*>(start)+3*dx;
        }

        static inline void * xshift4(void *start, unit_t dx)
        {
            return static_cast<uint32_t*>(start)+dx;
        }

        static inline void * xshift8(void *start, unit_t dx)
        {
            return static_cast<uint64_t*>(start)+dx;
        }

        static inline void * xshift16(void *start, unit_t dx)
        {
            return static_cast<uint64_t*>(start)+(2*dx);
        }

        bitmap::xshift_proc get_xshift(const unit_t depth)
        {
            switch (depth) {
                case 1:
                    return xshift1;
                case 2:
                    return xshift2;
                case 3:
                    return xshift3;
                case 4:
                    return xshift4;
                case 8:
                    return xshift8;
                case 16:
                    return xshift16;

                default:
                    break;
            }
            throw exception("bitmap: xshift%ld not implemented", long(depth));
        }


        bitmap:: bitmap(const unit_t D, const unit_t W, const unit_t H) :
        patch( vertex(0,0), vertex(check_w(W)-1,check_h(H)-1)),
        depth( check_d(D) ),
        w( width.x ),
        h( width.y ),
        pitch(w*depth),
        stride(pitch),
        entry(0),
        model(memory_is_global),
        xshift(get_xshift(depth))
        {
            allocated = pitch*h;
            entry     = memory::kind<memory::global>::acquire(allocated);
        }


        void *bitmap:: get_line(const unit_t j) throw()
        {
            assert(j>=0);
            assert(j<h);
            return static_cast<uint8_t*>(entry) + (j*stride);
        }

        const void *bitmap:: get_line(const unit_t j) const throw()
        {
            assert(j>=0);
            assert(j<h);
            return static_cast<uint8_t*>(entry) + (j*stride);
        }


        void *bitmap:: get(const unit_t i, const unit_t j) throw()
        {
            assert(i>=0);
            assert(i<w);
            assert(j>=0);
            assert(j<h);
            return xshift(static_cast<uint8_t*>(entry) + (j*stride),i);
        }

        const void *bitmap:: get(const unit_t i, const unit_t j) const throw()
        {
            assert(i>=0);
            assert(i<w);
            assert(j>=0);
            assert(j<h);
            return xshift(static_cast<uint8_t*>(entry) + (j*stride),i);
        }


        void bitmap:: ldz() throw()
        {
            for(unit_t j=0;j<h;++j)
            {
                memset( get_line(j), 0, pitch);
            }
        }


    }
}
