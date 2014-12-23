#include "yocto/gfx/bitmap.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace gfx;

YOCTO_UNIT_TEST_IMPL(bitmap)
{
    bitmap b1(2,200,100);
    bitmap b2(b1,NULL);
    
    rectangle r3(10, 10, 100, 50);
    bitmap    b3(b2, &r3);
    
}
YOCTO_UNIT_TEST_DONE()
