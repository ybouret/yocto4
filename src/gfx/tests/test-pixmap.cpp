#include "yocto/utest/run.hpp"
#include "yocto/gfx/pixmap.hpp"
#include "yocto/gfx/rgb.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/code/utils.hpp"
using namespace yocto;
using namespace gfx;


namespace
{

    template <typename T>
    static inline void fill1(pixmap<T> &px)
    {
        for(unit_t j=0;j<px.h;++j)
        {
            for(unit_t i=0;i<px.w;++i)
            {
                px[j][i] = T(1);
            }
        }
    }

    template <typename T>
    static inline void do_pixmap(pixmap<T> &b)
    {
        std::cerr << "depth=" << b.d << ", width=" << b.w << ", height=" << b.h << std::endl;
        b.ldz();
        b.flip_vert();

        std::cerr << " |_hard copy" << std::endl;
        bitmap::pointer p( new pixmap<T>(b,NULL) );
        std::cerr << " |_soft copy" << std::endl;
        pixmap<T>       s(p,NULL);

        std::cerr << " |_using rectangles" << std::endl;
        for(size_t k=1;k<=50;++k)
        {
            const unit_t x = alea_lt(b.w);
            const unit_t y = alea_lt(b.h);
            const unit_t w = max_of<size_t>(1,alea_leq(b.w-1-x));
            const unit_t h = max_of<size_t>(1,alea_leq(b.h-1-y));
            const rectangle r(x,y,w,h);
            pixmap<T> d(b,&r);
            pixmap<T> q(p,&r);
            std::cerr << ".";
        }
        std::cerr << std::endl;


    }

}

YOCTO_UNIT_TEST_IMPL(pixmap)
{
    pixmap<uint8_t> p1(100,50);
    do_pixmap(p1);

    pixmap<int16_t> p2(200,150);
    do_pixmap(p2);

    pixmap<rgb_t>   p3(512,256);
    do_pixmap(p3);

    pixmap<rgba_t>   p4(512,256);
    do_pixmap(p4);

    pixmap<float> pf(300,450);
    do_pixmap(pf);

    pixmap<float> pf2(p3,rgb2gsf<rgb_t>);
    pixmap<float> pf3(p4,rgb2gsf<rgba_t>);

    pixmap<double> pd(100,250);
    do_pixmap(pd);


}
YOCTO_UNIT_TEST_DONE()
