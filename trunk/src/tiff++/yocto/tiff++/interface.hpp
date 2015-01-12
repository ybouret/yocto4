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
        };
    }
}

#endif
