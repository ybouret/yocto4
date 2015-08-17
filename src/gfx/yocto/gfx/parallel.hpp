#ifndef YOCTO_GFX_PARALLEL_INCLUDED
#define YOCTO_GFX_PARALLEL_INCLUDED 1

#include "yocto/gfx/bitmap.hpp"
#include "yocto/threading/simd.hpp"

namespace yocto
{
    namespace gfx
    {

        void prepare_simd_contexts(threading::SIMD &simd,
                                   const bitmap    &bmp,
                                   const bool       full);

    }
}

#endif
