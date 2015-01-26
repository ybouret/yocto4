#include "yocto/gfx/image/tiff.hpp"
#include "yocto/gfx/image/png.hpp"

#include "yocto/gfx/rawpix-io.hpp"

#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace gfx;

YOCTO_UNIT_TEST_IMPL(tiff)
{
    arc_ptr<tiff_format> fmt( new tiff_format() );
    arc_ptr<png_format>  png( new png_format()  );

    if(argc>1)
    {
        const string   filename = argv[1];
        const uint32_t nd = fmt->count_directories(filename);
        std::cerr << "Found #image=" << nd << std::endl;
        for(uint32_t i=0;i<nd;++i)
        {
            arc_ptr<bitmap> bmp( fmt->load_bitmap(filename, 3, io_conv::put_rgb,NULL,i) );
            png->save( vformat("img%u.png", unsigned(i)), *bmp, io_conv::get_rgb, NULL, NULL);
        }
    }
}
YOCTO_UNIT_TEST_DONE()
