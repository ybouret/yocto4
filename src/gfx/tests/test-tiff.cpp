#include "yocto/gfx/image/tiff.hpp"
#include "yocto/gfx/image/png.hpp"
#include "yocto/gfx/image/jpeg.hpp"

#include "yocto/gfx/rawpix-io.hpp"
#include "yocto/gfx/ops/contrast.hpp"
#include "yocto/gfx/ops/hist.hpp"
#include "yocto/gfx/ops/blob.hpp"
#include "yocto/gfx/named-colors.hpp"

#include "yocto/utest/run.hpp"



using namespace yocto;
using namespace gfx;


static rgba_t LOCAL_COLORS[8];

static inline rgba_t get_rgba_from_blob(const void *addr, const void *args)
{
    assert(args);
    const size_t    value = *(const size_t *)addr;
    const clusters &cls   = *(const clusters *)args;


    size_t count = 1;
    for(clusters::const_iterator i=cls.begin();i!=cls.end();++i,++count)
    {
        const cluster &c = **i;
        if(value==c.uuid)
        {
            //return LOCAL_COLORS[(value-1)%8];
            return rgba_t(255,255,255);
        }
        if(count>=10)
            break;
    }

    return rgba_t(0,0,0);

    if(value<=10)
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


YOCTO_UNIT_TEST_IMPL(tiff)
{
    arc_ptr<tiff_format>  fmt( new tiff_format() );
    arc_ptr<png_format>   png( new png_format()  );
    arc_ptr<jpeg_format>  jpg( new jpeg_format()  );

    LOCAL_COLORS[0] = named_color::get("white");
    LOCAL_COLORS[1] = named_color::get("red");
    LOCAL_COLORS[2] = named_color::get("green");
    LOCAL_COLORS[3] = named_color::get("blue");
    LOCAL_COLORS[4] = named_color::get("yellow");
    LOCAL_COLORS[5] = named_color::get("cyan");
    LOCAL_COLORS[6] = named_color::get("magenta");
    LOCAL_COLORS[7] = named_color::get("orange");


    if(argc>1)
    {
        const string   filename = argv[1];
        const uint32_t nd       = fmt->count_directories(filename);
        std::cerr << "Found #image=" << nd << std::endl;
        for(uint32_t i=0;i<nd;++i)
        {
            bitmap::pointer bmp( fmt->load_bitmap(filename, 3,  io_conv::put_rgb, NULL,i) );
            pixmap3         pxm(bmp);
            pixmapf         pgs(pxm,rgb2gsf<rgb_t>);

            maximum_contrast(pgs);

            //png->save( vformat("img%u.png", unsigned(i)), pgs, image::get_gsf, NULL, NULL);
            

            histogram H;
            H.compute_from(pgs);
            const size_t t = H.threshold();
            std::cerr << "threshold=" << t << std::endl;


            pixmapf mask0(pgs.w,pgs.h);
            threshold::apply(mask0,t,pgs, threshold::keep_white);

            clusters cls;

            blob B0(mask0,cls,true);
            cls.sort();
            std::cerr << "#blobs0=" << B0.count << std::endl;

            //jpg->save( vformat("img%u.jpg", unsigned(i)), *bmp, io_conv::get_rgb, NULL, NULL);
            //jpg->save( vformat("img%u.jpg", unsigned(i)), mask0, image::get_gsf, NULL, NULL);
            jpg->save( vformat("img%u.jpg", unsigned(i)), B0, get_rgba_from_blob,&cls,NULL);

        }
    }
}
YOCTO_UNIT_TEST_DONE()
