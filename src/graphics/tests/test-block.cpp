#include "yocto/graphics/image/png.hpp"
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



        IMG["PNG"].save("p0.png", *p0, NULL);
        IMG["PNG"].save("p1.png", *p1, NULL);



        //compute_corr(U,V,*p0,*p1,3);
        
        break;
    }
    
}
YOCTO_UNIT_TEST_DONE()

