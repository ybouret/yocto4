#include "yocto/utest/run.hpp"
#include "yocto/gfx/image/jpeg.hpp"
#include "yocto/gfx/image/png.hpp"
#include "yocto/gfx/image/tiff.hpp"

using namespace yocto;
using namespace gfx;

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

    IMG.declare( new png_format() );
    IMG.declare( new jpeg_format() );
    IMG.declare( new tiff_format() );

    show_format( IMG["PNG"]  );
    show_format( IMG["JPEG"] );
    show_format( IMG["TIFF"] );
    
    
}
YOCTO_UNIT_TEST_DONE()
