#include "yocto/gfx/image.hpp"
#include "yocto/gfx/image/jpeg.hpp"
#include "yocto/gfx/image/png.hpp"
#include "yocto/gfx/image/tiff.hpp"

#include "yocto/gfx/ops/samples.hpp"
#include "yocto/gfx/ops/histogram.hpp"
#include "yocto/gfx/ops/blur.hpp"
#include "yocto/gfx/ops/fft.hpp"
#include "yocto/gfx/ops/filter.hpp"
#include "yocto/gfx/ops/differential.hpp"
#include "yocto/gfx/ops/edges.hpp"
#include "yocto/parallel/basic.hpp"
#include "yocto/code/rand.hpp"

#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace gfx;

#define DURATION 1

#include "yocto/sys/timings.hpp"

#define _EFF parallel::efficiency(speedup,xps_par.size())
#define TMX_FOR(FUNCTION) do {                                    \
std::cerr << "\nTiming " << #FUNCTION  << std::endl;              \
YOCTO_TIMINGS(tmx,DURATION,FUNCTION(xps_seq));                    \
const double seq_speed = tmx.speed;                               \
std::cerr << "|_seq_speed= " << seq_speed << std::endl;           \
YOCTO_TIMINGS(tmx,DURATION,FUNCTION(xps_par));                    \
const double par_speed = tmx.speed;                               \
const double speedup   = par_speed/seq_speed;                     \
std::cerr << "|_par_speed= " << par_speed   << std::endl;         \
std::cerr << "  |_speedup    = " << speedup << std::endl;         \
std::cerr << "  |_efficiency = " << _EFF    << '%' << std::endl;  \
} while(false)

YOCTO_UNIT_TEST_IMPL(perf)
{
    YOCTO_GFX_DECL_FORMAT(jpeg);
    YOCTO_GFX_DECL_FORMAT(png);
    YOCTO_GFX_DECL_FORMAT(tiff);

    timings tmx;

    imageIO          &IMG = image::instance();
    if(argc>1)
    {
        std::cerr << "-- Load image" << std::endl;
        const string filename = argv[1];
        pixmap1      img1( IMG.load1(filename,NULL) );
        pixmap3      img3( IMG.load3(filename,NULL) );
        pixmap4      img4( IMG.load4(filename,NULL) );
        pixmapf      imgf( IMG.loadf(filename,NULL) );

        xpatches xps_par(imgf,true);
        xpatches xps_seq(imgf);
        std::cerr << "xps_par.size=" << xps_par.size() << ", xps_seq.size=" << xps_seq.size() << std::endl;

        const unit_t w=imgf.w;
        const unit_t h=imgf.h;

        pixmap1 tgt1(w,h);
        pixmap3 tgt3(w,h);
        pixmap4 tgt4(w,h);
        pixmapf tgtf(w,h);

        {
            samples<RGB>     s3;
            pixmaps<uint8_t> channels(3,w,h);
#define     SPLIT_SAMPLES(XPS) s3.split(channels,img3,XPS)
            TMX_FOR(SPLIT_SAMPLES);
#define     MERGE_SAMPLES(XPS) s3.merge(channels,tgt3,XPS)
            TMX_FOR(MERGE_SAMPLES);
        }

        {
#define     SEPARATE1(XPS) separate( threshold::keep_foreground,tgt1,img1,XPS)
            TMX_FOR(SEPARATE1);
#define     SEPARATE3(XPS) separate( threshold::keep_foreground,tgt3,img3,XPS)
            TMX_FOR(SEPARATE3);
#define     SEPARATE4(XPS) separate( threshold::keep_foreground,tgt4,img4,XPS)
            TMX_FOR(SEPARATE4);
#define     SEPARATEf(XPS) separate( threshold::keep_foreground,tgtf,imgf,XPS)
            TMX_FOR(SEPARATEf);
        }

        {
            pixmaps<real_t> channels(3,w,h);
            differential    drvs;
#define     GRAD1(XPS) drvs.compute(tgt1,img1,channels,differential::gradient,XPS);
            TMX_FOR(GRAD1);
#define     GRAD3(XPS) drvs.compute(tgt3,img3,channels,differential::gradient,XPS);
            TMX_FOR(GRAD3);
#define     GRAD4(XPS) drvs.compute(tgt4,img4,channels,differential::gradient,XPS);
            TMX_FOR(GRAD4);
#define     GRADf(XPS) drvs.compute(tgtf,imgf,channels,differential::gradient,XPS);
            TMX_FOR(GRADf);
        }

        {
            edges Edg(w,h);
#define     EDGES1(XPS) Edg.build_from(img1,XPS)
            TMX_FOR(EDGES1);
#define     EDGES3(XPS) Edg.build_from(img3,XPS)
            TMX_FOR(EDGES3);
#define     EDGES4(XPS) Edg.build_from(img4,XPS)
            TMX_FOR(EDGES4);
#define     EDGESf(XPS) Edg.build_from(imgf,XPS)
            TMX_FOR(EDGESf);
        }

        {
            blur B(2.0f);
#define     BLURf(XPS) B.apply(tgtf,imgf,XPS)
            TMX_FOR(BLURf);
#define     BLUR3(XPS) B.apply(tgt3,img3,XPS)
            TMX_FOR(BLUR3);
        }
    }
}
YOCTO_UNIT_TEST_DONE()
