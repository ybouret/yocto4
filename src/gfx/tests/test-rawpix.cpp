#include "yocto/gfx/rawpix.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/code/bzset.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;
using namespace gfx;

#define SHOW(B) std::cerr << #B << "\t: " << (B).w << "x" << (B).h << "x" << (B).d << ",\tpitch=" << (B).pitch << ", stride=" << (B).stride << std::endl

namespace
{
    template <typename PIXMAP>
    static inline void fill_pixmap(PIXMAP &P ) throw()
    {
        for(unit_t j=0;j<P.h;++j)
        {
            for(unit_t i=0;i<P.w;++i)
            {
                typename PIXMAP::type &p = P[j][i];
                bzset(p);
                uint8_t *q = (uint8_t *) &p;
                for(size_t k=0;k<sizeof(typename PIXMAP::type);++k)
                {
                    q[k] = uint8_t( alea_leq(255) );
                }
            }
        }

    }
    
    template <typename PIXMAP>
    static inline void clear_pixmap(PIXMAP &P) throw()
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
}


YOCTO_UNIT_TEST_IMPL(rawpix)
{
    
    pixmap3 p3(200,100); SHOW(p3); fill_pixmap(p3);
    pixmap3 p4(300,150); SHOW(p4); fill_pixmap(p4);
    pixmapf f3( p3, rgb2gs<rgb_t>  ); SHOW(f3);
    pixmapf f4( p4, rgb2gs<rgba_t> ); SHOW(f4);
    
    save_ppm("p3.ppm", p3, addr2rgb3 );
    save_ppm("p4.ppm", p4, addr2rgb4 );

    save_ppm("f3.ppm", f3, addr2rgbf );
    save_ppm("f4.ppm", f4, addr2rgbf );

    
}
YOCTO_UNIT_TEST_DONE()
