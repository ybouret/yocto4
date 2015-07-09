#include "yocto/utest/run.hpp"
#include "yocto/gfx/bitmap.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/code/utils.hpp"
using namespace yocto;
using namespace gfx;

namespace
{
    static inline void do_bitmap(bitmap &b)
    {
        std::cerr << "depth=" << b.d << ", width=" << b.w << ", height=" << b.h << std::endl;
        b.ldz();
        b.flip_vert();

        std::cerr << " |_hard copy" << std::endl;
        bitmap::pointer p( new bitmap(b,NULL) );
        std::cerr << " |_soft copy" << std::endl;
        bitmap          s(p,NULL);

        std::cerr << " |_using rectangles" << std::endl;
        for(size_t k=1;k<=50;++k)
        {
            const unit_t x = alea_lt(b.w);
            const unit_t y = alea_lt(b.h);
            const unit_t w = max_of<size_t>(1,alea_leq(b.w-1-x));
            const unit_t h = max_of<size_t>(1,alea_leq(b.h-1-y));
            const rectangle r(x,y,w,h);
            bitmap d(b,&r);
            bitmap q(p,&r);
            std::cerr << ".";
        }
        std::cerr << std::endl;
    }
}



YOCTO_UNIT_TEST_IMPL(bitmap)
{
    bitmap B1(1,100,50);
    do_bitmap(B1);

    bitmap B2(2,200,100);
    do_bitmap(B2);

    bitmap B3(3,150,52);
    do_bitmap(B3);

    bitmap B4(4,120,51);
    do_bitmap(B4);

    bitmap B8(8,99,23);
    do_bitmap(B8);

    bitmap B16(16,77,100);
    do_bitmap(B16);

}
YOCTO_UNIT_TEST_DONE()
