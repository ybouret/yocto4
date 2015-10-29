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

    rectangle rect(5,6, 10, 10);
    bitmap bmp1a(bmp1);
    bitmap bmp1b(bmp1,rect);

    bitmap bmp2a(bmp2);
    bitmap bmp2b(bmp2,rect);

    bitmap bmp3a(bmp3);
    bitmap bmp3b(bmp3,rect);

    bitmap bmp4a(bmp4);
    bitmap bmp4b(bmp4,rect);

    bitmap bmp8a(bmp8);
    bitmap bmp8b(bmp8,rect);

    bitmap bmp16a(bmp16);
    bitmap bmp16b(bmp16,rect);

    bitmap *shared = new bitmap(1,100,100);
    bitmap sbmp1a(shared);
    bitmap sbmp1b(shared,rect);


}
YOCTO_UNIT_TEST_DONE()
