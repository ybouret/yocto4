#include "yocto/graphics/image/png.hpp"
#include "yocto/graphics/image/tiff.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/graphics/region.hpp"

using namespace yocto;
using namespace graphics;

static inline void load_block( array<float> &value, const pixmapf &pxm, const unit_t x, const unit_t y, const unit_t r)
{
    assert( (2*r+1)*(2*r+1) == value.size() );
    size_t idx=0;
    for(unit_t dy=-r;dy<=r;++dy)
    {
        const unit_t ytmp = y+dy;
        for(unit_t dx=-r;dx<=r;++dx)
        {
            value[++idx] = pxm[ytmp][x+dx];
        }
    }

}


static inline
void compute_corr(pixmapf       &U,
                  pixmapf       &V,
                  const pixmapf &p0,
                  const pixmapf &p1,
                  const unit_t   r)
{
    assert(r>=0);
    const unit_t w = U.w;
    const unit_t h = U.h;
    U.ldz();
    V.ldz();
    const size_t  dim = r*2+1;
    const size_t  np = dim*dim;
    vector<float> src(np);
    vector<float> tgt(np);

    unit_t xmin = dim;
    unit_t xmax = w-(dim);
    unit_t ymin = dim;
    unit_t ymax = h-(dim);

    for(unit_t y=ymin;y<ymax;++y)
    {
        for(unit_t x=xmin;x<xmax;++x)
        {
            load_block(src,p0, x, y, r);

            for(unit_t dy=-r;dy<=r;++dy)
            {
                for(unit_t dx=-r;dx<=r;++dx)
                {
                    load_block(tgt,p1,x+dx,y+dy,r);
                }
            }

        }
    }
}


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
    pixmapf U(w,h);
    pixmapf V(w,h);
    region  Sqr; Sqr.load_square(20);
    region  Dsk; Dsk.load_disk(20);
    region  Reg; Reg.load_disk(20); Reg.shift(0, 15); Reg.load_square(10); Reg.simplify();
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

        Sqr.tag(*p0, 0, 0, 0.25);
        Sqr.tag(*p0, 0, h, 0.50);
        Sqr.tag(*p0, w, h, 0.75);
        Sqr.tag(*p0, w, 0, 1.00);

        Dsk.tag(*p1, 0, 0, 0.25);
        Dsk.tag(*p1, 0, h, 0.50);
        Dsk.tag(*p1, w, h, 0.75);
        Dsk.tag(*p1, w, 0, 1.00);

        Reg.tag(*p0, w/2, h/2, 0.9);

        IMG["PNG"].save("p0.png", *p0, NULL);
        IMG["PNG"].save("p1.png", *p1, NULL);



        //compute_corr(U,V,*p0,*p1,3);
        
        break;
    }
    
}
YOCTO_UNIT_TEST_DONE()

