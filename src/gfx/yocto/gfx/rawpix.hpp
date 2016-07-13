#ifndef YOCTO_GFX_RAWPIX_INCLUDED
#define YOCTO_GFX_RAWPIX_INCLUDED 1

#include "yocto/gfx/color/yuv.hpp"
#include "yocto/gfx/pixmap.hpp"

namespace yocto
{
    namespace gfx
    {
        typedef pixmap<uint8_t>  pixmap1;
        typedef pixmap<float>    pixmapf;
        typedef pixmap<double>   pixmapd;
        typedef pixmap<RGB>      pixmap3;
        typedef pixmap<RGBA>     pixmap4;
        typedef pixmap<cplx_t>   pixmapz;
        typedef pixmap<YUV>      yuvmap;
    }
}

#endif
