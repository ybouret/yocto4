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

        static inline int __GetWidth(void *handle, uint32_t *w ) throw()
        {
            assert(handle);
            assert(w);
            return TIFFGetField( (TIFF *)handle, TIFFTAG_IMAGEWIDTH, w);
        }

        static inline int __GetHeight(void *handle, uint32_t *h ) throw()
        {
            assert(handle);
            assert(h);
            return TIFFGetField( (TIFF *)handle, TIFFTAG_IMAGELENGTH, h);
        }

        static inline
        int    __ReadRGBAImage(void *handle, const uint32_t w, const uint32_t h, uint32_t *raster) throw()
        {
            assert(handle);
            assert(raster);
            return TIFFReadRGBAImage((TIFF *)handle, w, h, raster,0);
        }

        static inline
        int __SetDirectory(void *handle, uint32_t n) throw()
        {
            assert(handle);
            return TIFFSetDirectory((TIFF*)handle, n);
        }

        static inline
        uint32_t __CountDirectories(void *handle)
        {
            assert(handle);
            TIFF *tiff = (TIFF *)handle;
            uint32_t count = 0;
            do
            {
                ++count;
            }
            while( TIFFReadDirectory(tiff) );
            return count;
        }


#define LINK(FUNCTION) api->FUNCTION = __##FUNCTION

        YOCTO_EXPORT void YOCTO_API YOCTO_RTLD_LOADER(I_TIFF *api) throw()
        {
            assert(api);
            LINK(Open);
            LINK(Close);
            LINK(ReadDirectory);
            LINK(GetWidth);
            LINK(GetHeight);
            LINK(ReadRGBAImage);
            LINK(SetDirectory);
            LINK(CountDirectories);
        }
    }
}


