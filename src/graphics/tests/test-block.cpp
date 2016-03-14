#include "yocto/graphics/image/png.hpp"
#include "yocto/graphics/image/jpeg.hpp"
#include "yocto/graphics/image/tiff.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/graphics/ops/fft.hpp"
#include "yocto/graphics/ops/bilinear.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/graphics/ramp/cold_to_very_hot.hpp"
#include "yocto/graphics/ramp/grey.hpp"
#include "yocto/graphics/ramp/orange.hpp"
#include "yocto/graphics/region.hpp"
#include "yocto/parallel/basic.hpp"
#include "yocto/parallel/field.hpp"

using namespace yocto;
using namespace graphics;


static inline
size_t split_length( const unit_t length, const unit_t ws ) throw()
{
    unit_t nw = 4;
    while(nw*ws<length) ++nw;
    return --nw;
}


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


    const unit_t ws = 16;
    const size_t nw = split_length(w, ws);
    const size_t nh = split_length(h, ws);
    std::cerr << "window_size=" << ws    << std::endl;
    std::cerr << "nw         =" << nw    << std::endl;
    std::cerr << "nh         =" << nh    << std::endl;
    std::cerr << "#regions   =" << nw*nh << std::endl;

    const parallel::patch2D   z2( coord2D(0,0), coord2D(nw-1,nh-1) );
    parallel::field2D<region> Regions(z2);

    for(size_t j=0;j<nh;++j)
    {
        unit_t y_off = 0;
        unit_t y_len = h;
        parallel::basic_split(j, nh, y_off, y_len);
        for(size_t i=0;i<nw;++i)
        {
            unit_t x_off = 0;
            unit_t x_len = w;
            parallel::basic_split(i, nw, x_off, x_len);
            region &rr = Regions[j][i];
            vertex pos;
            for(unit_t y=y_off,ny=y_len;ny>0;++y,--ny)
            {
                pos.y = y;
                for(unit_t x=x_off,nx=x_len;nx>0;++x,--nx)
                {
                    pos.x = x;
                    regxel *r = new regxel(pos);
                    rr.push_back(r);
                }
            }
        }

    }

    get_gsz          z2gs;
    //cold_to_very_hot rmp;
    //float &vmin = rmp.vmin;
    //float &vmax = rmp.vmax;

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

        for(size_t j=0;j<nh;++j)
        {
            for(size_t i=0;i<nw;++i)
            {
                region &rr = Regions[j][i];
                rr.load(*p0);
            }
        }



        break;
    }
    std::cerr << "sizeof(regxel)=" << sizeof(regxel) << std::endl;
    
    
}
YOCTO_UNIT_TEST_DONE()

