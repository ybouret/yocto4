#include "yocto/tiff++/io.hpp"
#include "yocto/exception.hpp"

#include <iostream>
namespace yocto
{
    namespace gfx
    {
        
        tiff_reader:: ~tiff_reader() throw()
        {
            TIFFClose(tif);
            tif = 0;
            (size_t&)num = 0;
        }
        
        static inline
        TIFF * __open_tiff_r(const string &filename )
        {
            TIFF *tif = TIFFOpen(filename.c_str(), "r");
            if(!tif)
            {
                throw exception("TIFF: can't open '%s'", filename.c_str());
            }
            return tif;
        }
        
        static inline
        size_t __count_dirs( TIFF *tif )
        {
            assert(tif);
            std::cerr.flush();
            size_t dircount = 0;
            do {
                ++dircount;
                TIFFPrintDirectory(tif,stderr);
            } while (TIFFReadDirectory(tif));
            fflush(stderr);
            return dircount;
        }
        
        
        tiff_reader:: tiff_reader( const string &the_path ) :
        path(the_path),
        tif( __open_tiff_r(path) ),
        num( __count_dirs(tif)   )
        {
        }
    }
}
