#include "yocto/gfx/parallel.hpp"
#include "yocto/exception.hpp"
#include "yocto/core/mpi-split.hpp"

namespace yocto
{
    namespace gfx
    {

        patch:: patch(bitmap &user_bmp, const rectangle &user_rect) throw() :
        bmp(user_bmp),
        rect(user_rect)
        {

        }


        patch:: ~patch() throw() {}



        void patch:: setup(threading::SIMD &simd,
                           bitmap          &bmp,
                           const bool       full)
        {
            // set up
            unit_t xoffset=0,yoffset=0;
            unit_t xlength=bmp.w, ylength=bmp.h;
            if(!full)
            {
                xoffset =  1;
                yoffset =  1;
                xlength -= 2;
                ylength -= 2;
                if(xlength<0||ylength<0)
                {
                    throw exception("gfx.simd: bitmap is too small!");
                }
            }

            // prepare to cut
            core::mpi_split2D<unit_t> split(simd.size,xlength,ylength);

            for(size_t rank=0;rank<simd.size;++rank)
            {
                unit_t xoff = xoffset;
                unit_t yoff = yoffset;
                unit_t xlen = xlength;
                unit_t ylen = ylength;
                split(rank,xoff,yoff,xlen,ylen);
                const rectangle rect(xoff,yoff,xlen,ylen);
                threading::SIMD::Context &ctx = simd[rank];
                ctx.build<patch,bitmap&,const rectangle&>(bmp,rect);
            }


            
        }
        
    }
    
}
