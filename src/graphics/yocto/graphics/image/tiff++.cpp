#include "yocto/graphics/image/tiff++.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/memory/global.hpp"
#include "tiffio.h"

namespace yocto
{
    namespace graphics
    {
        
        I_TIFF:: Raster:: ~Raster() throw()
        {
            cleanup();
        }
        
        I_TIFF:: Raster:: Raster() throw() :
        data(0),
        size(0)
        {
        }
        
        void I_TIFF:: Raster:: cleanup() throw()
        {
            memory::kind<memory::global>::release_as<uint32_t>(data,(size_t&)size);
        }
        
        void I_TIFF:: Raster:: startup(const size_t n)
        {
            if(n>size)
            {
                cleanup();
                (size_t&)size = n;
                data = memory::kind<memory::global>::acquire_as<uint32_t>((size_t&)size);
            }
        }

        
        I_TIFF:: I_TIFF(const string &filename) :
        handle( TIFFOpen(filename.c_str(),"r") )
        {
            if(!handle) throw imported::exception("TIFFOpen","couldn't open '%s'", filename.c_str());
        }
        
        I_TIFF:: ~I_TIFF() throw()
        {
            assert(handle);
            TIFFClose((TIFF *)handle);
            handle=0;
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
        
        void I_TIFF:: ReadRBGAImage(Raster &raster)
        {
            const int    w   = GetWidth();
            const int    h   = GetHeight();
            raster.startup(w*h);
            
            if( 1 != TIFFReadRGBAImage((TIFF*)handle, w, h, raster.data) )
            {
                throw imported::exception("TIFFReadRGBAImage","failure...");
            }
        }
        
        
    }
}
