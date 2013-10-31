#ifndef YOCTO_GFX_IMAGE_INCLUDED
#define YOCTO_GFX_IMAGE_INCLUDED 1

#include "yocto/gfx/bitmap.hpp"
#include "yocto/gfx/rgb.hpp"
#include "yocto/ios/ostream.hpp"

namespace yocto
{
    namespace gfx
    {
        
        struct image
        {
            typedef rgb_t (*addr2rgba)(const void *addr, const void *args);
            static void save(ios::ostream &fp,
                             const bitmap &bmp,
                             int           fmt,
                             addr2rgba     proc,
                             const void   *args);
        };
        
    }
}

#endif
