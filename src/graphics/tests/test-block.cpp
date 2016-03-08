#include "yocto/graphics/image/png.hpp"
#include "yocto/graphics/image/jpeg.hpp"
#include "yocto/graphics/image/tiff.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/graphics/ops/fft.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/graphics/ramp/cold_to_very_hot.hpp"
#include "yocto/graphics/ramp/grey.hpp"

using namespace yocto;
using namespace graphics;



YOCTO_UNIT_TEST_IMPL(block)
{
    image &IMG = image::instance();

    auto_ptr<pixmapf> p0;
    auto_ptr<pixmapf> p1;

    tiff_format *TIF = new tiff_format();
    IMG.declare(TIF);
    IMG.declare(new  png_format() );
    IMG.declare(new  jpeg_format() );
    const imageIO &gfx = IMG;

    if(argc<2) throw exception("need one TIFF file");
    const string   filename = argv[1];
    const uint32_t nd       = TIF->count_directories(filename);
    if(nd<=1)
        throw exception("need at least two images TIFF...");

    put_gsf cb;
    bitmap *bmp = TIF->load_bitmap(filename, 4, cb, 0);
    p0.reset( new pixmapf(bmp) );

    const unit_t w = p0->w;
    const unit_t h = p0->h;

    fft sourceA(w,h);
    fft sourceB(w,h);

    pixmapz zprod(sourceA.data.w,sourceA.data.h);
    pixmapf rprod(sourceA.core.w,sourceA.core.h);

    get_gsz          z2gs;
    cold_to_very_hot rmp;

    for(size_t id=0;id<nd-1;++id)
    {
        if(id>0)
        {
            assert(p1.is_valid());
            p0.reset( p1.yield() );
        }

        {
            bitmap *bmp = TIF->load_bitmap(filename, 4, cb, id+1);
            p1.reset( new pixmapf(bmp) );
        }


        gfx.save("p0.png", *p0, NULL);
        gfx.save("p1.png", *p1, NULL);

        sourceA.load(*p0);
        sourceA.dispatch(); //! dispatch full content...
        gfx.save("p0d.png", sourceA.data, z2gs, NULL);

        sourceB.load(*p1);
        sourceB.dispatch( rectangle(w/2,h/2,32,32) );
        gfx.save("p1d.png", sourceB.data, z2gs, NULL);

        sourceA.forward();
        sourceB.forward();
        
        for(unit_t y=0;y<sourceA.data.h;++y)
        {
            for(unit_t x=0;x<sourceA.data.w;++x)
            {
                zprod[y][x] = sourceA.data[y][x] * sourceB.data[y][x].conj();
            }
        }

        fft::__reverse(zprod);

        rmp.vmin=zprod[0][0].re;
        rmp.vmax=rmp.vmin;
        for(unit_t y=0;y<rprod.h;++y)
        {
            for(unit_t x=0;x<rprod.w;++x)
            {
                const float tmp = zprod[y][x].re;
                if(tmp<rmp.vmin) rmp.vmin=tmp;
                if(tmp>rmp.vmax) rmp.vmax=tmp;
                rprod[y][x] = tmp;
            }
        }
        std::cerr << "vmin=" << rmp.vmin << ", vmax=" << rmp.vmax << std::endl;

        gfx.save("corr.png", rprod, rmp, NULL);

        rmp.vmin=zprod[0][0].mod();
        rmp.vmax=rmp.vmin;
        for(unit_t y=0;y<rprod.h;++y)
        {
            for(unit_t x=0;x<rprod.w;++x)
            {
                const float tmp = zprod[y][x].mod();
                if(tmp<rmp.vmin) rmp.vmin=tmp;
                if(tmp>rmp.vmax) rmp.vmax=tmp;
                rprod[y][x] = tmp;
            }
        }
        std::cerr << "vmin=" << rmp.vmin << ", vmax=" << rmp.vmax << std::endl;

        gfx.save("peak.png", rprod, rmp, NULL);



        break;
    }
    
}
YOCTO_UNIT_TEST_DONE()

