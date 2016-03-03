#include "yocto/graphics/image/png.hpp"
#include "yocto/graphics/image/tiff.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/graphics/region.hpp"
#include "yocto/container/matrix.hpp"

using namespace yocto;
using namespace graphics;




YOCTO_UNIT_TEST_IMPL(block)
{
    std::cerr << "sizeof(regxel)=" << sizeof(regxel) << std::endl;
    std::cerr << "sizeof(list<regxel>::node_type)=" << sizeof(list<regxel>::node_type) << std::endl;
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
    const unit_t nr = w*h;
    std::cerr << "need " << nr*2 << " regions!" << std::endl;
    pixmapf U(w,h);
    pixmapf V(w,h);

    std::cerr << "creating matrices..." << std::endl;

    /*
     matrix<region> regA(h,w);
     matrix<region> regB(h,w);
    std::cerr << "Filling Matrices..." << std::endl;
    for(unit_t j=1;j<=h;++j)
    {
        std::cerr << ".";
        std::cerr.flush();
        for(unit_t i=1;i<=w;++i)
        {
            regA[j][i].load_disk(20);
            regB[j][i].load_disk(20);
        }
    }
    std::cerr << std::endl;
     */

    matrix< vector<float> > regA(h,w);
    for(unit_t j=1;j<=h;++j)
    {
        std::cerr << ".";
        std::cerr.flush();
        for(unit_t i=1;i<=w;++i)
        {
            regA[j][i].reserve(100);
        }
    }
    std::cerr << std::endl;





    region  Sqr; Sqr.load_square(20);
    region  Dsk; Dsk.load_disk(20);
    region  Reg; Reg.load_disk(20); Reg.shift(0, 15); Reg.load_square(10); Reg.simplify(); Reg.center();

    

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

