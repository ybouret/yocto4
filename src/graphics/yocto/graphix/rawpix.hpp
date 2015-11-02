#ifndef YOCTO_GRAPHIX_RAWPIX_INCLUDED
#define YOCTO_GRAPHIX_RAWPIX_INCLUDED 1

#include "yocto/graphix/pixmap.hpp"
#include "yocto/graphix/rgb.hpp"

namespace yocto
{
    namespace graphix
    {
        typedef pixmap<uint8_t>  pixmap1;
        typedef pixmap<float>    pixmapf;
        typedef pixmap<double>   pixmapd;
        typedef pixmap<RGB>      pixmap3;
        typedef pixmap<RGBA>     pixmap4;
        
    }
}

#endif
