#include "yocto/gfx/pixmap.hpp"
#include "yocto/gfx/rgb.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/code/bzset.hpp"

using namespace yocto;
using namespace gfx;

#define SHOW(B) std::cerr << #B << "\t: " << (B).w << "x" << (B).h << "x" << (B).d << ",\tpitch=" << (B).pitch << ", stride=" << (B).stride << std::endl

template <typename PIXMAP>
static inline void clear_pixmap(PIXMAP &P)
{
    for(unit_t j=0;j<P.h;++j)
    {
        for(unit_t i=0;i<P.w;++i)
        {
            typename PIXMAP::type &p = P[j][i];
            bzset(p);
        }
    }
}

YOCTO_UNIT_TEST_IMPL(pixmap)
{
    std::cerr << "sizeof(rgb_t) =" << sizeof(rgb_t)  << std::endl;
    std::cerr << "sizeof(rgba_t)=" << sizeof(rgba_t) << std::endl;
    
    pixmap<rgb_t>    p1(200,100); SHOW(p1);
    pixmap<rgba_t>   p2(200,100); SHOW(p2);
    pixmap<int32_t>  p3(300,100); SHOW(p3);
    pixmap<uint16_t> p4(100,200); SHOW(p4);
    
    clear_pixmap(p1);
    clear_pixmap(p2);
    clear_pixmap(p3);
    clear_pixmap(p4);

}
YOCTO_UNIT_TEST_DONE()
