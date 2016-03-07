#ifndef YOCTO_GRAPHIX_RAWPIX_INCLUDED
#define YOCTO_GRAPHIX_RAWPIX_INCLUDED 1

#include "yocto/graphics/pixmap.hpp"
#include "yocto/graphics/rgb.hpp"

namespace yocto
{
    namespace graphics
    {
        typedef pixmap<uint8_t>  pixmap1;
        typedef pixmap<float>    pixmapf;
        typedef pixmap<double>   pixmapd;
        typedef pixmap<RGB>      pixmap3;
        typedef pixmap<RGBA>     pixmap4;
        typedef pixmap<cplx_t>   pixmapz;
    }
}

#endif
