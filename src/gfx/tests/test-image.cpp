#include "yocto/utest/run.hpp"
#include "yocto/gfx/image.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(image)
{
    gfx::image &IMG = gfx::image::instance();
    
    std::cerr << IMG.get_name() << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()


#include "yocto/gfx/image/jpeg.hpp"
#include "yocto/ptr/auto.hpp"

YOCTO_UNIT_TEST_IMPL(load_jpeg)
{
    if(argc>1)
    {
        const string     filename = argv[1];
        gfx::jpeg_format jpg;
        auto_ptr<gfx::surface> surf( jpg.load(filename, gfx::ARGB32()) );
    }
}
YOCTO_UNIT_TEST_DONE()
