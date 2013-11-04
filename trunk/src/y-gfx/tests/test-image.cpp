#include "yocto/utest/run.hpp"
#include "yocto/gfx/image.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(image)
{
    gfx::image &img = gfx::image::instance();
    std::cerr << "testing " << img.get_name() << std::endl;
}
YOCTO_UNIT_TEST_DONE()
