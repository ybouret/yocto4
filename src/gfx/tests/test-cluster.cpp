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


#include "yocto/gfx/ops/blob.hpp"

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
    const  rgb_t &c = *(const rgb_t *)addr;
    return rgba_t(c.r,c.g,c.b);
}

static rgba_t LOCAL_COLORS[8];

static inline rgba_t get_rgba_from_blob(const void *addr, const void *args)
{
    const size_t  value = *(const size_t *)addr;
    if(value<=0)
    {
        return rgba_t(0,0,0);
    }
    else
    {
        //const size_t  count = *(const size_t *)args;
        //const named_color &nc = named_color::reg[ (value-1) % named_color::num ];
        //return rgba_t(nc.r,nc.g,nc.b);
        return LOCAL_COLORS[(value-1)%8];
    }
}

#include "yocto/sort/quick.hpp"

#define SHOW_FLAG(FLAG) std::cerr << #FLAG << "\t=\t" << (FLAG) << std::endl; flags.push_back(FLAG)

YOCTO_UNIT_TEST_IMPL(cluster)
{
    image &IMG = image::instance();
    
    IMG.declare( new png_format()  );
    IMG.declare( new jpeg_format() );
    
    LOCAL_COLORS[0] = named_color::get("white");
    LOCAL_COLORS[1] = named_color::get("red");
    LOCAL_COLORS[2] = named_color::get("green");
    LOCAL_COLORS[3] = named_color::get("blue");
    LOCAL_COLORS[4] = named_color::get("yellow");
    LOCAL_COLORS[5] = named_color::get("cyan");
    LOCAL_COLORS[6] = named_color::get("magenta");
    LOCAL_COLORS[7] = named_color::get("grey");

    for(int i=1;i<argc;++i)
    {
        const string path = argv[i];
        const bitmap::pointer  bmp( IMG.load(path,4,put_rgba_dup,NULL,NULL) );
        pixmap4                pxm(bmp);
        const string           root    = vfs::get_base_name(path);
        std::cerr << path << ": " << pxm.w << "x" << pxm.h << std::endl;
        
        pixmapf                pgs(pxm,rgb2gsf<rgba_t>);
        maximum_contrast(pgs);
        
        histogram H;
        H.compute_from(pgs);
        const size_t t = H.threshold();
        std::cerr << "threshold=" << t << std::endl;
        
        pixmapf mask0(pgs.w,pgs.h);
        threshold::apply(mask0,t,pgs, threshold::keep_black);
        
        
        {
            const string outname = root + ".mask0.png";
            IMG["PNG"].save(outname,mask0, get_rgba_dup,NULL,NULL);
        }
        
        pixmapf mask1(pgs.w,pgs.h);
        threshold::apply(mask1,t,pgs, threshold::keep_white);
        
        
        {
            const string outname = root + ".mask1.png";
            IMG["PNG"].save(outname,mask1, get_rgba_dup,NULL,NULL);
        }
        
        
        
        clusters cls;
        
        blob B0(mask0,cls,true);
        std::cerr << "#blobs0=" << B0.count << std::endl;
        
        {
            const string outname = root + ".blob0.png";
            IMG["PNG"].save(outname,B0, get_rgba_from_blob,&B0.count,NULL);
        }
        
        blob B1(mask1,cls,true);
        std::cerr << "#blobs1=" << B1.count << std::endl;
        
        {
            const string outname = root + ".blob1.png";
            IMG["PNG"].save(outname,B1, get_rgba_from_blob,&B1.count,NULL);
        }

        
        
        
    }
    
}
YOCTO_UNIT_TEST_DONE()

