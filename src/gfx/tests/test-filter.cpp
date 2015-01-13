#include "yocto/gfx/image.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/gfx/image/png.hpp"
#include "yocto/gfx/image/jpeg.hpp"
#include "yocto/gfx/rawpix-io.hpp"
#include "yocto/gfx/ops/filter.hpp"

#include "yocto/fs/vfs.hpp"
#include "yocto/gfx/ops/contrast.hpp"
#include "yocto/gfx/ops/hist.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/gfx/named-colors.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;
using namespace gfx;


YOCTO_UNIT_TEST_IMPL(filter)
{
    image &IMG = image::instance();

    IMG.declare( new png_format()  );
    IMG.declare( new jpeg_format() );


    for(int i=1;i<argc;++i)
    {
        const string path = argv[i];
        const bitmap::pointer  bmp( IMG.load(path,4,io_conv::put_rgba,NULL) );
        pixmap4                pxm(bmp);
        const string           root    = vfs::get_base_name(path);
        std::cerr << path << ": " << pxm.w << "x" << pxm.h << std::endl;

        pixmap3 out(pxm.w,pxm.h);

        filter<float> f;

        apply_filter<uint8_t,uint8_t,float,3>(out,pxm,f);

    }

}
YOCTO_UNIT_TEST_DONE()
