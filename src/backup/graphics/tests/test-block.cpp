#include "yocto/graphics/image/png.hpp"
#include "yocto/graphics/image/jpeg.hpp"
#include "yocto/graphics/image/tiff.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/graphics/ops/bilinear.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/graphics/ramp/cold_to_very_hot.hpp"
#include "yocto/graphics/ramp/grey.hpp"
#include "yocto/graphics/ramp/orange.hpp"
#include "yocto/graphics/region.hpp"
#include "yocto/parallel/basic.hpp"
#include "yocto/parallel/field.hpp"
#include "yocto/graphics/piv/zone.hpp"

using namespace yocto;
using namespace graphics;





using parallel::coord2D;

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
    pixmap3      pxm(w,h);

    PIV::Zones zones(*p0,16);
    std::cerr << "zones       =" << zones << std::endl;
    std::cerr << "owned_memory=" << zones.owned_memory << std::endl;
    std::cerr << "allocated   =" << zones.allocated()  << std::endl;

    pixmapf zcr(zones.width.x,zones.width.y);

    get_gsz          z2gs;
    cold_to_very_hot rmp;
    float &vmin = rmp.vmin;
    float &vmax = rmp.vmax;

    for(size_t id=0;id<nd-1;++id)
    {
        if(id>0)
        {
            assert(p1.is_valid());
            //p0.reset( p1.yield() );
            p0.steal(p1);
        }

        {
            bitmap *bmp = TIF->load_bitmap(filename, 4, cb, id+1);
            p1.reset( new pixmapf(bmp) );
        }


        gfx.save("p0.png", *p0, NULL);
        gfx.save("p1.png", *p1, NULL);

        size_t ic=0;
        vmin = 0;
        vmax = 0;
        for(unit_t j=0;j<zones.H;++j)
        {
            for(unit_t i=0;i<zones.W;++i)
            {
                const RGB &c    = named_color::fetch(++ic);
                PIV::Zone &zone = zones[j][i];
                zone.load(*p0);
                for(unit_t y=zone.lower.y;y<=zone.upper.y;++y)
                {
                    for(unit_t x=zone.lower.x;x<=zone.upper.x;++x)
                    {
                        const float I = (*p0)[y][x];
                        pxm[y][x] = RGB(gist::float2byte(I*c.r),
                                        gist::float2byte(I*c.g),
                                        gist::float2byte(I*c.b) );
                    }
                }
                const float tmp = zone.correlate_with(*p1, vertex(0,0) );
                zcr[j][i] = tmp;
                vmin = min_of(tmp,vmin);
                vmax = max_of(tmp,vmax);
            }

        }
        gfx.save("pz.png", pxm, NULL);
        std::cerr << "vmin=" << vmin << ", vmax=" << vmax << std::endl;
        gfx.save("zcr.png", zcr, rmp, NULL);
        

        break;
    }
    std::cerr << "sizeof(Zone)=" << sizeof(PIV::Zone) << std::endl;

    
}
YOCTO_UNIT_TEST_DONE()

