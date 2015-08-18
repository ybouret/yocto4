#ifndef YOCTO_GFX_PARALLEL_INCLUDED
#define YOCTO_GFX_PARALLEL_INCLUDED 1

#include "yocto/gfx/rectangle.hpp"
#include "yocto/threading/simd.hpp"
#include "yocto/core/mpi-split.hpp"

//#include <iostream>

namespace yocto
{
    namespace gfx
    {


        //! a patch interface
        class ipatch
        {

        public:
            const rectangle rect;
            void           *source;
            void           *target;
            void           *params;

            //! compute the parameters from w and h
            static void setup_parallel_metrics(unit_t      &xoff,
                                               unit_t      &yoff,
                                               unit_t      &xlen,
                                               unit_t      &ylen,
                                               const unit_t w,
                                               const unit_t h,
                                               const bool   full);

            explicit ipatch(const rectangle &r) throw();
            virtual ~ipatch() throw();

            //! assuming contexts are built from ipatch
            static void dispatch(threading::SIMD &simd, void *src,void *tgt,void *prm);


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(ipatch);
        };


        //! DATATYPE is a kind of patch...
        template <typename DATATYPE> inline
        static void setup_contexts(threading::SIMD &simd,
                                   const unit_t     w,
                                   const unit_t     h,
                                   const bool       full)
        {
            unit_t       xoff = 0;
            unit_t       yoff = 0;
            unit_t       xlen = 0;
            unit_t       ylen = 0;
            const size_t size = simd.size;
            ipatch::setup_parallel_metrics(xoff,yoff,xlen,ylen,w,h,full);
            core::mpi_split2D<unit_t> split(size,xlen,ylen);


            for(size_t rank=0;rank<size;++rank)
            {
                unit_t xoffset = xoff;
                unit_t yoffset = yoff;
                unit_t xlength = xlen;
                unit_t ylength = ylen;
                split(rank,xoffset,yoffset,xlength,ylength);
                const rectangle rect(xoffset,yoffset,xlength,ylength);
                //std::cerr << "\t" << size << "." << rank << ":";
                //std::cerr << "[" << rect.x << ":" << rect.xout-1 << "] * ";
                //std::cerr << "[" << rect.y << ":" << rect.yout-1 << "]";
                //std::cerr << std::endl;
                threading::SIMD::Context &ctx = simd[rank];
                ctx.build<DATATYPE,const rectangle&>(rect);
            }
        }



    }
}

#endif
