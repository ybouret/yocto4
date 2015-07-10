#ifndef YOCTO_GFX_RAWPIX_INCLUDED
#define YOCTO_GFX_RAWPIX_INCLUDED 1

#include "yocto/gfx/pixmap.hpp"
#include "yocto/gfx/rgb.hpp"

namespace yocto
{
    namespace gfx
    {
        
        typedef pixmap<rgb_t>  pixmap3;
        typedef pixmap<rgba_t> pixmap4;
        typedef pixmap<float>  pixmapf;
        typedef pixmap<double> pixmapd;
        
        

             
    }
}

#endif
