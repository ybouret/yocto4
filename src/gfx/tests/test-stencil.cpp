#include "yocto/gfx/image.hpp"
#include "yocto/gfx/image/jpeg.hpp"
#include "yocto/gfx/image/png.hpp"
#include "yocto/gfx/image/tiff.hpp"

#include "yocto/gfx/ops/stencil.hpp"
#include "yocto/code/rand.hpp"

#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace gfx;

YOCTO_UNIT_TEST_IMPL(stencil)
{
    YOCTO_GFX_DECL_FORMAT(jpeg);
    YOCTO_GFX_DECL_FORMAT(png);
    YOCTO_GFX_DECL_FORMAT(tiff);

    imageIO          &IMG = image::instance();
    stencil<float>   Sf;
    stencil<RGB>     S3;
    stencil<int>     Si;

    Sf.load_smooth();
    std::cerr << "smooth_f=" << Sf << std::endl;
    Si.load_sharpen();
    std::cerr << "sharpen_i=" << Si << std::endl;
    Sf.load_gauss();
    std::cerr << "gauss_f=" << Sf << std::endl;
    
    if(argc>1)
    {

        const string filename = argv[1];
        pixmap3      img3( IMG.load3(filename,NULL) );
        pixmapf      imgf( IMG.loadf(filename,NULL) );

    }
}
YOCTO_UNIT_TEST_DONE()