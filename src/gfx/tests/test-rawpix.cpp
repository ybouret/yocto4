#include "yocto/gfx/rawpix-io.hpp"
#include "yocto/gfx/ops/contrast.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/code/bzset.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/gfx/named-colors.hpp"

using namespace yocto;
using namespace gfx;

#define SHOW(B) std::cerr << #B << "\t: " << (B).w << "x" << (B).h << "x" << (B).d << ",\tpitch=" << (B).pitch << ", stride=" << (B).stride << std::endl

namespace
{
    template <typename PIXMAP>
    static inline void fill_pixmap(PIXMAP &P ) throw()
    {
        size_t count = 0;
        size_t a     = alea_lt(named_color::num);
        size_t b     = alea_lt(named_color::num);
        if(b<a)
        {
            cswap(a,b);
        }
        assert(b>=a);
        const size_t n = b+1-a;
        for(unit_t j=0;j<P.h;++j)
        {
            for(unit_t i=0;i<P.w;++i)
            {
                typename PIXMAP::type &p = P[j][i];
                bzset(p);
                const named_color     &nc =named_color::reg[a+(count%n)];
                const rgb_t             c(nc.r,nc.g,nc.b);
                typename PIXMAP::type  C(c);
                p = C;
                ++count;
#if 0
                uint8_t *q = (uint8_t *) &p;
                for(size_t k=0;k<sizeof(typename PIXMAP::type);++k)
                {
                    q[k] = uint8_t( alea_leq(255) );
                }
#endif
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
    pixmapf f3( p3, rgb2gsf<rgb_t>  ); SHOW(f3);
    pixmapf f4( p4, rgb2gsf<rgba_t> ); SHOW(f4);
    
    save_ppm("p3.ppm", p3 );
    save_ppm("p4.ppm", p4);
    
    save_ppm("f3.ppm", f3);
    save_ppm("f4.ppm", f4);
    
    maximum_contrast(f3);
    maximum_contrast(f4);
    
    save_ppm("m3.ppm", f3);
    save_ppm("m4.ppm", f4);

    pixmapf f3b(p3, rgb2gsf<rgb_t> ); SHOW(f3b);
    pixmapd d3(f3b.w,f3b.h);          SHOW(d3);
    
    maximum_contrast(d3,f3b);
    save_ppm("d3.ppm",d3);
    
               
}
YOCTO_UNIT_TEST_DONE()
