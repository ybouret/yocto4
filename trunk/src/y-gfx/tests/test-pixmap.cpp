#include "yocto/utest/run.hpp"
#include "yocto/gfx/pixmap.hpp"


using namespace yocto;


YOCTO_UNIT_TEST_IMPL(pixmap)
{
    std::cerr << "sizeof(pixmap<int>)=" << sizeof(gfx::pixmap<int>) << std::endl;
    std::cerr << "sizeof(pixmap<double>)=" << sizeof(gfx::pixmap<double>) << std::endl;

    gfx::pixmap<uint32_t> pix(100,100);
}
YOCTO_UNIT_TEST_DONE()
