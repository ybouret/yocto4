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
        
        
        YOCTO_EXPORT void YOCTO_API load_tiff(I_TIFF *api) throw()
        {
            //std::cerr << "Loading TIFF API" << std::endl;
            assert(api);
            api->Open  = __Open;
            api->Close = __Close;
        }
    }
}


