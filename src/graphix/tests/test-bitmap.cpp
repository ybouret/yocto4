#include "yocto/graphix/bitmap.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace graphix;

YOCTO_UNIT_TEST_IMPL(bitmap)
{
    bitmap bmp1(1,100,20);
    bitmap bmp2(2,110,30);
    bitmap bmp3(3,120,40);
    bitmap bmp4(4,130,50);
    bitmap bmp8(8,140,60);
    bitmap bmp16(16,150,70);

}
YOCTO_UNIT_TEST_DONE()
