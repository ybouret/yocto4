#ifndef YOCTO_GRAPHICS_OPS_FFT_INCLUDED
#define YOCTO_GRAPHICS_OPS_FFT_INCLUDED 1

#include "yocto/graphics/rawpix.hpp"

namespace yocto
{
    namespace graphics
    {
        struct fft
        {
            //! must be a full pixmap, not shared
            static void forward( pixmapz &img ) throw();
            static void reverse( pixmapz &img ) throw();
        };
    }
}

#endif
