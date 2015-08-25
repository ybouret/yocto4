#ifndef YOCTO_GFX_PARALLEL_INCLUDED
#define YOCTO_GFX_PARALLEL_INCLUDED 1

#include "yocto/gfx/bitmap.hpp"
#include "yocto/threading/simd.hpp"
#include "yocto/parallel/setup.hpp"

namespace yocto
{
    namespace gfx
    {


        typedef parallel::coord2D coord2D;
        typedef parallel::patch2D patch2D;

        //! generic patch for bitmaps
        class patch
        {
        public:
            //! compute the parameters
            /**
             \param xoff set to 0 if full=true, 1 if full=false
             \param yoff set to 0 if full=true, 1 if full=false
             \param xlen input value, -2 if full=false
             \param ylen input value, -2 if full=false
             \param full set to false if borders are to be excluded
             Throw if xlen<=0 of ylen<=0
             */
            static void setup_parallel_metrics(coord2D     &offset,
                                               coord2D     &length,
                                               const unit_t w,
                                               const unit_t h,
                                               const bool   full);
            const rectangle area;


            explicit patch(const  patch2D &p) throw();
            virtual ~patch() throw();

            template <typename PATCH>
            static inline void setup(slots_of<PATCH> &patches,
                                     const size_t     cpus,
                                     const bitmap    &bmp,
                                     const bool       full)
            {
                assert(patches.is_empty());
                coord2D offset;
                coord2D length;
                // construct the source patch
                patch::setup_parallel_metrics(offset, length, bmp.w, bmp.h,full);
                const patch2D zone(offset,parallel::__coord_dec(offset+length));

                // construct the sub patches
                parallel::setup_patches(patches, cpus, zone);
            }

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(patch);
        };
        
        //! generic patches
        class patches : public dynamic_slots<patch>
        {
        public:
            explicit patches(size_t n=0);
            virtual ~patches() throw();

            void build_for(const size_t cpus,const bitmap &bmp,const bool full);

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(patches);
        };

    }
}

#endif
