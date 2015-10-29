#include "yocto/graphix/image/png.hpp"
#include "yocto/graphix/image/jpeg.hpp"
#include "yocto/graphix/image/tiff.hpp"
#include "yocto/graphix/rawpix.hpp"

#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace graphix;

namespace
{
    
    static void show_format( const image::format &fmt )
    {
        std::cerr << "<" << fmt.name << ">" << std::endl;
        const char **exts = fmt.extensions();
        for(size_t i=0;;++i)
        {
            const char *ext = exts[i];
            if(!ext) break;
            std::cerr << "\tmay load " << ext << std::endl;
        }
    }
    
}


YOCTO_UNIT_TEST_IMPL(img)
{
    image &IMG = image::instance();
    
    IMG.declare( new png_format()  );
    IMG.declare( new jpeg_format() );
    IMG.declare( new tiff_format() );
    
    show_format( IMG["PNG"]  );
    show_format( IMG["JPEG"] );
    show_format( IMG["TIFF"] );
    
    const image::format &PNG = IMG["PNG"];
    
    if(argc>1)
    {
        const string filename = argv[1];
        // load
        std::cerr << "Loading RGBA" << std::endl;
        pixmap4 p4( IMG.load4(filename, NULL));
        
        std::cerr << "Loading RGB" << std::endl;
        pixmap3 p3( IMG.load3(filename, NULL));
        
        std::cerr << "Loading BYTE" << std::endl;
        pixmap1 p1( IMG.load1(filename, NULL));
        
        std::cerr << "Loading FLOAT" << std::endl;
        pixmapf pf( IMG.loadf(filename, NULL));



    }
    
}
YOCTO_UNIT_TEST_DONE()
