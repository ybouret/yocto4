#ifndef YOCTO_GFX_PARALLEL_INCLUDED
#define YOCTO_GFX_PARALLEL_INCLUDED 1

#include "yocto/gfx/rectangle.hpp"
#include "yocto/threading/simd.hpp"
#include "yocto/parallel/split.hpp"

namespace yocto
{
    namespace gfx
    {

        typedef parallel::patch2D patch2D;
        typedef parallel::coord2D coord2D;

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
            static void setup_parallel_metrics(unit_t      &xoff,
                                               unit_t      &yoff,
                                               unit_t      &xlen,
                                               unit_t      &ylen,
                                               const unit_t w,
                                               const unit_t h,
                                               const bool   full);
            const rectangle area;
            const void     *source;
            void           *target;
            const void     *params;

            explicit patch(const patch2D &p) throw();
            virtual ~patch() throw();


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(patch);
        };

#if 0
        //! a patch interface
        class ipatch
        {

        public:
            const rectangle rect;
            const void     *source;
            void           *target;
            const void     *params;

            //! compute the parameters from w, h and full
            static void setup_parallel_metrics(unit_t      &xoff,
                                               unit_t      &yoff,
                                               unit_t      &xlen,
                                               unit_t      &ylen,
                                               const unit_t w,
                                               const unit_t h,
                                               const bool   full);

            explicit ipatch(const rectangle &r) throw();
            virtual ~ipatch() throw();

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(ipatch);
        };


        //! DATATYPE is a kind of patch...
        template <typename DATATYPE> inline
        void setup_contexts(threading::SIMD &simd,
                            const unit_t     w,
                            const unit_t     h,
                            const bool       full)
        {
            unit_t       xoff = 0;
            unit_t       yoff = 0;
            unit_t       xlen = 0;
            unit_t       ylen = 0;
            const size_t size = simd.size;
            // get the workload
            ipatch::setup_parallel_metrics(xoff,yoff,xlen,ylen,w,h,full);

            // prepare the workload balance
            core::mpi_split2D<unit_t> split(size,xlen,ylen);


            for(size_t rank=0;rank<size;++rank)
            {
                unit_t xoffset = xoff;
                unit_t yoffset = yoff;
                unit_t xlength = xlen;
                unit_t ylength = ylen;
                split(rank,xoffset,yoffset,xlength,ylength);
                const rectangle           rect(xoffset,yoffset,xlength,ylength);
                threading::SIMD::Context &ctx = simd[rank];
                ctx.build<DATATYPE,const rectangle&>(rect);
            }
        }

        //! DATATYPE is a kind of patch...
        template <typename DATATYPE> inline
        void reset_contexts(threading::SIMD &simd, const void *src,void *tgt,const void *prm)
        {
            for(size_t i=0;i<simd.size;++i)
            {
                DATATYPE &p = simd.get<DATATYPE>(i);
                p.source = src;
                p.target = tgt;
                p.params = prm;
            }
            
        }
#endif
        
    }
}

#endif
