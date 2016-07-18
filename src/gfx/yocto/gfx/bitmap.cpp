#include "yocto/gfx/bitmap.hpp"
#include "yocto/exception.hpp"
#include "yocto/error.hpp"

#include <cstdlib>
#include <cerrno>
#include <cstring>

namespace yocto
{
    namespace gfx
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
                case 12:
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

        static inline void * xshift12(void *start, unit_t dx)
        {
            return static_cast<uint8_t*>(start)+12*dx;
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

                case 12:
                    return xshift12;

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

        bitmap:: bitmap(void *data, const unit_t D, const unit_t W, const unit_t H, const unit_t S ) :
        patch( vertex(0,0), vertex(check_w(W)-1,check_h(H)-1)),
        depth( check_d(D) ),
        w( width.x ),
        h( width.y ),
        pitch(w*depth),
        stride(S),
        entry(0),
        model(memory_from_user),
        xshift(get_xshift(depth))
        {
            if(stride<pitch)
                throw exception("bitmap: invalid stride=%ld<pitch=%ld for user data",long(stride),long(pitch));
            if(!data)
                throw exception("bitmap: no user data");
            entry = data;
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

        // hard copy
        bitmap:: bitmap(const bitmap &bmp) :
        patch( bmp ),
        depth( bmp.depth ),
        w(bmp.w),
        h(bmp.h),
        pitch(bmp.pitch),
        stride(bmp.stride),
        entry(0),
        model(memory_is_global),
        xshift(bmp.xshift)
        {

            // allocate memory
            allocated = pitch*h;
            entry     = memory::kind<memory::global>::acquire(allocated);

            // copy
            copy(bmp);

        }

        // hard copy
        bitmap:: bitmap(const bitmap &bmp, const patch &area) :
        patch( vertex(0,0), vertex(area.width.x-1,area.width.y-1) ),
        depth( bmp.depth ),
        w(width.x),
        h(width.y),
        pitch(w*depth),
        stride(pitch),
        entry(0),
        model(memory_is_global),
        xshift(bmp.xshift)
        {

            // allocate memory
            allocated = pitch*h;
            entry     = memory::kind<memory::global>::acquire(allocated);

            // copy
            const size_t   tgt_stride = stride;
            const size_t   src_stride = bmp.stride;
            uint8_t       *tgt_entry  = static_cast<uint8_t *>(entry);
            const uint8_t *src_entry  = static_cast<const uint8_t *>( bmp.get(area.lower) );

            for(unit_t j=0;j<h;++j,tgt_entry+=tgt_stride,src_entry+=src_stride)
            {
                memcpy(tgt_entry, src_entry, pitch);
            }
            
        }

        static inline bitmap *__check(bitmap *bmp) throw()
        {
            if(!bmp)
            {
                libc::critical_error(ENOMEM, "bitmap shared copy");
            }
            return bmp;
        }

        bitmap:: bitmap(bitmap *bmp) throw() :
        patch( * __check(bmp) ),
        depth(bmp->depth),
        w(bmp->w),
        h(bmp->h),
        pitch(bmp->pitch),
        stride(bmp->stride),
        entry(bmp->entry),
        model(memory_is_shared),
        xshift(bmp->xshift)
        {
            shared = bmp;
            shared->withhold();
        }




        //! soft copy, shared data
        bitmap:: bitmap(bitmap *bmp, const patch &area) :
        patch( vertex(0,0), vertex(area.width.x-1,area.width.y-1) ),
        depth( __check(bmp)->depth ),
        w(width.x),
        h(width.y),
        pitch(w*depth),
        stride(bmp->stride),
        entry(0),
        model(memory_is_shared),
        xshift(get_xshift(depth))
        {

            if(!bmp->contains(area))
            {
                if(bmp->liberate()) delete bmp;
                throw exception("bitmap: shared copy invalid area");
            }
            entry = bmp->get(area.lower);
      
            shared = bmp;
            shared->withhold();

        }

        void bitmap:: copy(const bitmap &src) throw()
        {
            assert(src.w==w);
            assert(src.h==h);
            assert(src.stride==stride);

            const size_t   tgt_stride = stride;
            const size_t   src_stride = src.stride;
            uint8_t       *tgt_entry  = static_cast<uint8_t *>(entry);
            const uint8_t *src_entry  = static_cast<const uint8_t *>( src.entry );

            for(unit_t j=0;j<h;++j,tgt_entry+=tgt_stride,src_entry+=src_stride)
            {
                memcpy(tgt_entry, src_entry, pitch);
            }

        }

    }
}
