#include "yocto/graphics/image/png.hpp"
#include "yocto/graphics/image/jpeg.hpp"
#include "yocto/graphics/image/tiff.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/graphics/ops/fft.hpp"
#include "yocto/code/utils.hpp"

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
    //const unit_t nr = w*h;

    const unit_t aw = next_power_of_two(w);
    const unit_t ah = next_power_of_two(h);

    pixmapz za(aw,ah);
    pixmapz zb(aw,ah);
    pixmapz zz(aw,ah);
    pixmapf cr(w,h);

    get_rampf to_ramp;
    //get_gsf   to_gs;
    float &cmin = to_ramp.vmin;
    float &cmax = to_ramp.vmax;

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

        za.ldz();
        zb.ldz();
        for(unit_t j=0;j<h;++j)
        {
            for(unit_t i=0;i<w;++i)
            {
                za[j][i].re = (*p0)[j][i];
                zb[j][i].re = (*p1)[j][i];
            }
        }
        fft::forward(za);
        fft::forward(zb);
        for(unit_t j=0;j<ah;++j)
        {
            for(unit_t i=0;i<aw;++i)
            {
                zz[j][i] = za[j][i] * zb[j][i].conj();
            }
        }
        fft::reverse(zz);

        cmin = cmax = 0;
        for(unit_t j=0;j<h;++j)
        {
            for(unit_t i=0;i<w;++i)
            {
                const float cc = zz[j][i].re;
                cr[j][i] = cc;
                if(cc<cmin) cmin = cc;
                if(cc>cmax) cmax = cc;
            }
        }

        std::cerr << "cmin=" << cmin << std::endl;
        std::cerr << "cmax=" << cmax << std::endl;

        gfx.save("p0.png", *p0, NULL);
        gfx.save("p1.png", *p1, NULL);
        gfx.save("cr.png", cr,to_ramp, NULL);



        //compute_corr(U,V,*p0,*p1,3);
        
        break;
    }
    
}
YOCTO_UNIT_TEST_DONE()

