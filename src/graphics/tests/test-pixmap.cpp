#include "yocto/graphix/pixmap.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace graphix;

namespace {
struct abc { char a,b,c; };
}

YOCTO_UNIT_TEST_IMPL(pixmap)
{

    rectangle rect(5,6, 10, 10);
    pixmap<uint8_t>  p1(100,20),p1a(p1),p1b(p1,rect);
    pixmap<uint16_t> p2(100,20),p2a(p2),p2b(p2,rect);
    pixmap<uint32_t> p4(100,20),p4a(p4),p4b(p4,rect);
    pixmap<uint64_t> p8(100,20),p8a(p8),p8b(p8,rect);
    pixmap<abc>      p3(100,20),p3a(p3),p3b(p3,rect);

}
YOCTO_UNIT_TEST_DONE()

