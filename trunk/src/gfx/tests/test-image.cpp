#include "yocto/gfx/image.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/gfx/image/png.hpp"
#include "yocto/gfx/image/jpeg.hpp"
#include "yocto/fs/vfs.hpp"
#include "yocto/gfx/rawpix-io.hpp"

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
        {
            const string           outname = root + ".ppm";
            save_ppm(outname,pxm);
        }
        
        pixmapf                pgs(pxm,rgb2gsf<rgba_t>);
        {
            const string outname = root + ".gs.ppm";
            save_ppm(outname,pgs);
        }
        
        maximum_contrast(pgs);
        {
            const string outname = root + ".mc.ppm";
            save_ppm(outname,pgs);
        }
    }
    
}
YOCTO_UNIT_TEST_DONE()
