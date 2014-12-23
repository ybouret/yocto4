#include "yocto/gfx/bitmap.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace gfx;

#define SHOW(B) std::cerr << #B << "\t: " << (B).w << "x" << (B).h << "x" << (B).d << ",\tpitch=" << (B).pitch << ", stride=" << (B).stride << std::endl

YOCTO_UNIT_TEST_IMPL(bitmap)
{
    bitmap b1(2,200,100);  SHOW(b1);
    bitmap b2(b1,NULL);    SHOW(b2);
    
    rectangle r3(10, 10, 100, 50);
    bitmap    b3(b2, &r3); SHOW(b3);
    
    bitmap::pointer p( new bitmap(4,500,150) ); SHOW(*p);
    bitmap  q1(p,NULL); SHOW(q1);
    bitmap  q2(p,&r3);  SHOW(q2);
    
    
}
YOCTO_UNIT_TEST_DONE()
