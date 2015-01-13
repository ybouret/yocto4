#include "yocto/tiff++/interface.hpp"
#include "tiffio.h"

//#include <iostream>

namespace yocto
{
    namespace gfx
    {
        static inline
        void * __Open(const char *filename, const char *mode) throw()
        {
            TIFF *tiff = TIFFOpen(filename,mode);
            return tiff;
        }
        
        static inline
        void __Close(void *handle) throw()
        {
            assert(handle);
            TIFFClose((TIFF*)handle);
        }
        
        static inline int __ReadDirectory(void *handle) throw()
        {
            assert(handle);
            return TIFFReadDirectory((TIFF*)handle);
        }
        
#define LINK(FUNCTION) api->FUNCTION = __##FUNCTION
        
        YOCTO_EXPORT void YOCTO_API YOCTO_RTLD_LOADER(I_TIFF *api) throw()
        {
            //std::cerr << "Loading TIFF API" << std::endl;
            assert(api);
            LINK(Open);
            LINK(Close);
            LINK(ReadDirectory);
        }
    }
}


