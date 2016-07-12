#include "yocto/utest/run.hpp"

#include "yocto/graphics/image/png.hpp"
#include "yocto/graphics/image/jpeg.hpp"
#include "yocto/graphics/image/tiff.hpp"
#include "yocto/graphics/ops/filter.hpp"

using namespace yocto;
using namespace graphics;


namespace
{
    template <typename T>
    void run_filter(const pixmap<T> &src, xpatches &xps, threading::engine *server)
    {
        const image::format &PNG = (*image::location())["PNG"];
        const string id     = typeid(T).name();
        std::cerr << "Filtering for type=" << id << std::endl;
        const unit_t w = src.w;
        const unit_t h = src.h;
        filter<T> F;
        pixmap<T> org(src);
        pixmap<T> tgt(w,h);

        for(unsigned i=1;i<=4;++i)
        {
            F.apply(tgt,org,&F,&filter<T>::median,xps,server);
            const string med_id = "img" + id + vformat("_med%u.png",i);
            PNG.save(med_id,tgt,NULL);
            org.copy(tgt);
        }

        org.copy(src);
        for(unsigned i=1;i<=4;++i)
        {
            F.apply(tgt,org,&F,&filter<T>::average,xps,server);
            const string ave_id = "img" + id + vformat("_ave%u.png",i);
            PNG.save(ave_id,tgt,NULL);
            org.copy(tgt);
        }


        org.copy(src);
        for(unsigned i=1;i<=4;++i)
        {
            F.apply(tgt,org,&F,&filter<T>::expand,xps,server);
            const string ave_id = "img" + id + vformat("_exp%u.png",i);
            PNG.save(ave_id,tgt,NULL);
            org.copy(tgt);
        }

        org.copy(src);
        for(unsigned i=1;i<=4;++i)
        {
            F.apply(tgt,org,&F,&filter<T>::erode,xps,server);
            const string ave_id = "img" + id + vformat("_erd%u.png",i);
            PNG.save(ave_id,tgt,NULL);
            org.copy(tgt);
        }


    }

}

YOCTO_UNIT_TEST_IMPL(filter)
{
    image &IMG = image::instance();

    IMG.declare( new png_format()  );
    IMG.declare( new jpeg_format() );
    IMG.declare( new tiff_format() );

    const image::format &PNG = IMG["PNG"];

    threading::engine server(true);

    if(argc>1)
    {
        const string filename = argv[1];
        pixmapf imgf( IMG.loadf(filename,NULL) );
        pixmap1 img1( IMG.load1(filename,NULL) );
        pixmap3 img3( IMG.load3(filename,NULL) );
        pixmap4 img4( IMG.load4(filename,NULL) );

        PNG.save("imgf.png",imgf,NULL);
        PNG.save("img1.png",img1,NULL);
        PNG.save("img3.png",img3,NULL);
        PNG.save("img4.png",img4,NULL);

        xpatches xps;
        xpatch::create(xps, imgf, &server);

        if(true) run_filter(imgf,xps,&server);
        if(true) run_filter(img1,xps,&server);
        if(true) run_filter(img3,xps,&server);
        if(true) run_filter(img4,xps,&server);

    }

}
YOCTO_UNIT_TEST_DONE()
