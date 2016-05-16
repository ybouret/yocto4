#include "yocto/program.hpp"
#include "tiffio.h"

using namespace yocto;

YOCTO_PROGRAM_START()
{
    for(int i=1;i<argc;++i)
    {
        const string filename = argv[i];
        std::cerr << "-- " << filename << std::endl;
        TIFF *tif = TIFFOpen(filename.c_str(), "r");
        if(!tif)
        {
            throw exception("couldn't open %s", filename.c_str() );
        }


        int count = 0;
        do {
            ++count;
            uint32_t w=0;
            if( !TIFFGetField(tif,TIFFTAG_IMAGEWIDTH, &w) )
            {
                throw exception("couldn't read width");
            }
            std::cerr << "w=" << w << std::endl;

            uint32_t h=0;
            if( !TIFFGetField(tif,TIFFTAG_IMAGELENGTH, &h) )
            {
                throw exception("couldn't read width");
            }
            std::cerr << "h=" << w << std::endl;
            const size_t npx = w*h;
            uint32_t *raster = new uint32_t[npx];
            TIFFReadRGBAImage(tif,w,h,raster);
            delete []raster;

        } while( TIFFReadDirectory(tif) );
        std::cerr << "Found " << count << " entry(ies)" << std::endl;
        TIFFClose(tif);
    }
}
YOCTO_PROGRAM_END()
