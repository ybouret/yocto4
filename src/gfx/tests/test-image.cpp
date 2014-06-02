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

YOCTO_UNIT_TEST_IMPL(load_jpeg)
{
    if(argc>1)
    {
        const string     filename = argv[1];
        gfx::jpeg_format jpg;
        jpg.load(filename);
    }
}
YOCTO_UNIT_TEST_DONE()
