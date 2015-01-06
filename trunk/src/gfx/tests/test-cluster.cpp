#include "yocto/gfx/image.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/gfx/image/png.hpp"
#include "yocto/gfx/image/jpeg.hpp"
#include "yocto/fs/vfs.hpp"
#include "yocto/gfx/ops/contrast.hpp"
#include "yocto/gfx/ops/hist.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/gfx/named-colors.hpp"
#include "yocto/code/rand.hpp"


#include "yocto/gfx/cluster.hpp"

using namespace yocto;
using namespace gfx;

static inline void put_rgba_dup(void *addr, const rgba_t &C, const void *)
{
    *(rgba_t *)addr = C;
}

static inline rgba_t get_rgba_dup(const void *addr, const void *)
{
#if 1
    const float   f = *(const float *)addr;
    const uint8_t u = conv::to_byte(f);
    return rgba_t(u,u,u);
#endif
    //return *(const rgba_t *)addr;
}

static inline rgba_t get_rgba_from_rgb(const void *addr, const void *)
{
    const rgb_t &c = *(const rgb_t *)addr;
    return rgba_t(c.r,c.g,c.b);
}

#include "yocto/sort/quick.hpp"

#define SHOW_FLAG(FLAG) std::cerr << #FLAG << "\t=\t" << (FLAG) << std::endl; flags.push_back(FLAG)

YOCTO_UNIT_TEST_IMPL(cluster)
{
    image &IMG = image::instance();
    
    IMG.declare( new png_format()  );
    IMG.declare( new jpeg_format() );
    
    vector<int> flags(8,as_capacity);
    SHOW_FLAG(coord::__top);
    SHOW_FLAG(coord::__bottom);
    SHOW_FLAG(coord::__left);
    SHOW_FLAG(coord::__right);
    SHOW_FLAG(coord::__top|coord::__left);
    SHOW_FLAG(coord::__top|coord::__right);
    SHOW_FLAG(coord::__bottom|coord::__left);
    SHOW_FLAG(coord::__bottom|coord::__right);
    
    quicksort(flags);
    std::cerr << "flags=" << flags << std::endl;
    
    
    for(int i=1;i<argc;++i)
    {
        const string path = argv[i];
        const bitmap::pointer  bmp( IMG.load(path,4,put_rgba_dup,NULL) );
        pixmap4                pxm(bmp);
        const string           root    = vfs::get_base_name(path);
        std::cerr << path << ": " << pxm.w << "x" << pxm.h << std::endl;
        
        pixmapf                pgs(pxm,rgb2gsf<rgba_t>);
        maximum_contrast(pgs);
        
        histogram H;
        H.compute_from(pgs);
        const size_t t = H.threshold();
        std::cerr << "threshold=" << t << std::endl;
        pixmapf mask(pgs.w,pgs.h);
        threshold::apply(mask,t,pgs, threshold::keep_black);
        
        
        
        {
            const string outname = root + ".thr.png";
            IMG["PNG"].save(outname,mask, get_rgba_dup,NULL,NULL);
        }
        
        
        clusters cls;
        cls.build_from(mask);
        std::cerr << "#cluster=" << cls.size << std::endl;
        pixmap3 Q(mask.w,mask.h);
        

        for(const cluster *cl=cls.head;cl;cl=cl->next)
        {
            std::cerr << "\t size=" << cl->coords.size << " / #border=" << cl->border.size() << std::endl;
            const named_color &nc = named_color::reg[ alea_lt(named_color::num) ];
            const rgb_t        C(nc.r/2,nc.g/2,nc.b/2);
            const rgb_t        B(nc.r,nc.g,nc.b);
            for(const coord *cc = cl->coords.head;cc;cc=cc->next)
            {
                Q[cc->y][cc->x] = C;
            }
            for(size_t i=cl->border.size();i>0;--i)
            {
                const coord *cc = cl->border[i];
                Q[cc->y][cc->x] = B;
            }
        }
        
        {
            const string outname = root + ".main.png";
            IMG["PNG"].save(outname,Q, get_rgba_from_rgb,NULL,NULL);
        }
        
#if 0
        for(unit_t j=0;j<mask.h;++j)
        {
            for(unit_t i=0;i<mask.w;++i)
            {
                if(mask[j][i]>0.0f)
                {
                    mask[j][i] = 1.0f;
                }
            }
        }
        
        {
            const string outname = root + ".sat.png";
            IMG["PNG"].save(outname,mask, get_rgba_dup,NULL,NULL);
        }
        
        coord::clusters cl;
        cl.build_from(mask);
        
        std::cerr << "#cluster=" << cl.size << std::endl;
        pixmap3 Q(mask.w,mask.h);
        for(const coord::cluster *cc=cl.head;cc;cc=cc->next)
        {
            const coord::cluster &m = *cc;
            std::cerr << "\t " << m.size() << std::endl;
            
            const named_color &nc = named_color::reg[ alea_lt(named_color::num) ];
            const rgb_t        C(nc.r,nc.g,nc.b);
            for(size_t k=m.size();k>0;--k)
            {
                const coord c = m[k];
                Q[c.y][c.x] = C;
            }
            
        }
        
        
        {
            const string outname = root + ".main.png";
            IMG["PNG"].save(outname,Q, get_rgba_from_rgb,NULL,NULL);
        }
#endif
        
        
        
    }
    
}
YOCTO_UNIT_TEST_DONE()

