#include "yocto/graphics/image/tiff++.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/memory/global.hpp"
#include "tiffio.h"

namespace yocto
{
    namespace graphics
    {
        I_TIFF:: I_TIFF(const string &filename) :
        handle( TIFFOpen(filename.c_str(),"r") ),
        raster(0),
        dwords(0)
        {
            if(!handle) throw imported::exception("TIFFOpen","couldn't open '%s'", filename.c_str());
        }

        I_TIFF:: ~I_TIFF() throw()
        {
            assert(handle);
            cleanup();
            TIFFClose((TIFF *)handle);
            handle=0;
        }

        void I_TIFF::cleanup() throw()
        {
            memory::kind<memory::global>::release_as<uint32_t>(raster,dwords);
        }

        bool I_TIFF:: ReadDirectory()
        {
            assert(handle);
            return (1 == TIFFReadDirectory( (TIFF *)handle ));
        }

        int  I_TIFF:: GetWidth()
        {
            uint32_t w = 0;
            if( !TIFFGetField( (TIFF *)handle, TIFFTAG_IMAGEWIDTH, &w ) )
            {
                throw imported::exception("TIFFGetField","invalid width");
            }
            return int(w);
        }

        int  I_TIFF:: GetHeight()
        {
            uint32_t h = 0;
            if( !TIFFGetField( (TIFF *)handle, TIFFTAG_IMAGELENGTH, &h ) )
            {
                throw imported::exception("TIFFGetField","invalid height");
            }
            return int(h);
        }

        void I_TIFF:: SetDirectory(const size_t n)
        {
            if( 1 != TIFFSetDirectory((TIFF *)handle, n) )
            {
                throw imported::exception("TIFFSetDirectory", "couldn't set %u", unsigned(n));
            }
        }

        size_t I_TIFF::CountDirectories()
        {
            size_t count = 0;
            do { ++count; } while( ReadDirectory() );
            return count;
        }

        uint32_t *I_TIFF:: load_rgba()
        {
            cleanup();
            const int w = GetWidth();
            const int h = GetHeight();
            dwords = size_t(w*h);
            raster = memory::kind<memory::global>::acquire_as<uint32_t>(dwords);
            TIFFReadRGBAImage((TIFF*)handle, w, h, raster);
            return raster;
        }


    }
}
