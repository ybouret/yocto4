#include "yocto/gfx/parallel.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace gfx
    {
        void prepare_simd_contexts(threading::SIMD &simd,
                                   const bitmap    &bmp,
                                   const bool       full)
        {
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
            

        }

    }

}
