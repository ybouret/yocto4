#include "yocto/graphics/image/png.hpp"
#include "yocto/graphics/image/jpeg.hpp"
#include "yocto/graphics/image/tiff.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/graphics/ops/fft.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/graphics/ramp/cold_to_very_hot.hpp"
#include "yocto/graphics/ramp/grey.hpp"
#include "yocto/graphics/ramp/orange.hpp"

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

    const unit_t aw = next_power_of_two(w);
    const unit_t ah = next_power_of_two(h);

    pixmapz za(aw,ah);
    pixmapz zb(aw,ah);
    pixmapz zp(aw,ah);
    pixmapf cr(aw,ah);

    get_gsz          z2gs;
    cold_to_very_hot rmp;
    float &vmin = rmp.vmin;
    float &vmax = rmp.vmax;
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

        za.ldz();
        for(unit_t y=0;y<h;++y)
        {
            for(unit_t x=0;x<w;++x)
            {
                za[y][x].re = (*p0)[y][x];
            }
        }

        gfx.save("za.png", za, z2gs, NULL);
        fft::__forward(za);


        for(unit_t SQ=1;SQ<=16;SQ *= 2)
        //for(unit_t SQ=16;SQ<=16;SQ *= 2)
        {
            std::cerr << "SQ=" << SQ << std::endl;
            zb.ldz();
            const rectangle rect(w/2-SQ,h/2-SQ,2*SQ+1,2*SQ+1);
            for(unit_t y=rect.lower.y;y<=rect.upper.y;++y)
            {
                for(unit_t x=rect.lower.x;x<=rect.upper.x;++x)
                {
                    zb[y][x].re = (*p1)[y][x];
                }
            }

            gfx.save( vformat("zb%02d.png", int(SQ)), zb, z2gs, NULL);
            fft::__forward(zb);

            for(unit_t y=0;y<ah;++y)
            {
                for(unit_t x=0;x<aw;++x)
                {
                    zp[y][x] = za[y][x]* zb[y][x].conj();
                }
            }
            fft::__reverse(zp);

            vmin = vmax = zp[0][0].re;
            const unit_t sx = aw/2;
            const unit_t sy = ah/2;
            for(unit_t y=0;y<ah;++y)
            {
                for(unit_t x=0;x<aw;++x)
                {
                    const float tmp = zp[y][x].re;
                    if(tmp<vmin) vmin=tmp;
                    if(tmp>vmax) vmax=tmp;
                    cr[(y+sy)%ah][(x+sx)%aw] = tmp;
                }
            }
            std::cerr << "vmin=" << vmin << ", vmax=" << vmax << std::endl;
            gfx.save( vformat("cr%02d.png",int(SQ)),cr,rmp,NULL);
        }
        
        break;
    }
    
}
YOCTO_UNIT_TEST_DONE()

