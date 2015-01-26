#ifndef YOCTO_TIFF_INTERFACE_INCLUDED
#define YOCTO_TIFF_INTERFACE_INCLUDED 1

#include "yocto/rtld/export.hpp"

namespace yocto
{
    
    namespace gfx
    {
        struct I_TIFF
        {
            void * (*Open)(const char *filename, const char *mode);
            void   (*Close)(void *);
            int    (*ReadDirectory)(void*);
            int    (*GetWidth)(void *, uint32_t *w);
            int    (*GetHeight)(void *, uint32_t *h);
            int    (*ReadRGBAImage)(void *, const uint32_t w, const uint32_t h, uint32_t *raster);
            int    (*SetDirectory)(void *,uint32_t d);
        };
    }
}

#endif
