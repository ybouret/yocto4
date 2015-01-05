#include "yocto/gfx/image.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/gfx/image/png.hpp"
#include "yocto/gfx/image/jpeg.hpp"
#include "yocto/fs/vfs.hpp"
#include "yocto/gfx/rawpix-io.hpp"
#include "yocto/gfx/ops/contrast.hpp"
#include "yocto/gfx/ops/edge.hpp"
#include "yocto/gfx/ops/hist.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;
using namespace gfx;

static void show_format( const image::format &fmt )
{
    std::cerr << "<" << fmt.name << ">" << std::endl;
    const char **exts = fmt.extensions();
    for(size_t i=0;;++i)
    {
        const char *ext = exts[i];
        if(!ext) break;
        std::cerr << "\tmay load " << ext << std::endl;
    }
}

static inline void put_rgba_dup(void *addr, const rgba_t &C, const void *)
{
    *(rgba_t *)addr = C;
}

static inline rgba_t get_rgba_dup(const void *addr, const void *)
{
    const float   f = *(const float *)addr;
    const uint8_t u = conv::to_byte(f);
    return rgba_t(u,u,u);
}

YOCTO_UNIT_TEST_IMPL(image)
{
    image &IMG = image::instance();
    
    IMG.declare( new png_format() );
    IMG.declare( new jpeg_format() );
    
    show_format( IMG["PNG"]  );
    show_format( IMG["JPEG"] );
    
    for(int i=1;i<argc;++i)
    {
        const string           path = argv[i];
        const bitmap::pointer  bmp( IMG.load(path,4,put_rgba_dup,NULL) );
        pixmap4                pxm(bmp);
        const string           root    = vfs::get_base_name(path);
        std::cerr << path << ": " << pxm.w << "x" << pxm.h << std::endl;
        
        std::cerr << "-- saving as ppm" << std::endl;
        {
            const string           outname = root + ".ppm";
            save_ppm(outname,pxm);
        }
        
        std::cerr << "-- building gray scale" << std::endl;
        pixmapf                pgs(pxm,rgb2gsf<rgba_t>);
        {
            const string outname = root + ".gs.ppm";
            save_ppm(outname,pgs);
        }
        
        std::cerr << "-- maximum constrast v1" << std::endl;
        maximum_contrast(pgs);
        {
            const string outname = root + ".mc.ppm";
            save_ppm(outname,pgs);
        }
        
        std::cerr << "-- computing edges" << std::endl;
        pixmapf edge(pgs.w,pgs.h);
        compute_edge(edge,pgs);
        
        {
            const string outname = root + ".edge.png";
            IMG["PNG"].save(outname, edge, get_rgba_dup, NULL,NULL);
        }
        
        std::cerr << "-- building histogram" << std::endl;
        histogram H;
        compute_histogram(H,pgs);
        {
            const string outname = root + ".hist.dat";
            ios::ocstream fp(outname,false);
            for(unsigned i=0;i<256;++i)
            {
                fp("%u %g\n", i, H[i] );
            }
        }
        
        
    }
    
}
YOCTO_UNIT_TEST_DONE()
