#include "yocto/gfx/image.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/gfx/image/png.hpp"
#include "yocto/gfx/image/jpeg.hpp"
#include "yocto/fs/vfs.hpp"
#include "yocto/gfx/ops/contrast.hpp"
#include "yocto/gfx/ops/hist.hpp"
#include "yocto/ios/ocstream.hpp"

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

YOCTO_UNIT_TEST_IMPL(cluster)
{
    image &IMG = image::instance();
    
    IMG.declare( new png_format() );
    IMG.declare( new jpeg_format() );

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
        
        
    }

}
YOCTO_UNIT_TEST_DONE()

