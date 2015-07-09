#include "yocto/utest/run.hpp"
#include "yocto/gfx/bitmap.hpp"

using namespace yocto;
using namespace gfx;

namespace
{
    static inline void do_bitmap(bitmap &b)
    {
        b.ldz();
        b.flip_vert();
    }
}

YOCTO_UNIT_TEST_IMPL(bitmap)
{
    bitmap B1(1,100,50);
    do_bitmap(B1);
    
}
YOCTO_UNIT_TEST_DONE()
