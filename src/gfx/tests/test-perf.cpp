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

#include "yocto/code/rand.hpp"

#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace gfx;

#define DURATION 2

#include "yocto/sys/timings.hpp"

#define TMX_FOR(FUNCTION) do {                                    \
std::cerr << "\nTiming " << #FUNCTION  << std::endl;              \
YOCTO_TIMINGS(tmx,DURATION,FUNCTION(xps_seq));                    \
const double seq_speed = tmx.speed;                               \
std::cerr << "|_seq_speed= " << seq_speed << std::endl;           \
YOCTO_TIMINGS(tmx,DURATION,FUNCTION(xps_par));                    \
const double par_speed = tmx.speed;                               \
std::cerr << "|_par_speed= " << par_speed << std::endl;           \
std::cerr << "  |_speedup= " << par_speed/seq_speed << std::endl; \
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

        {
            samples<RGB>     s3;
            pixmaps<uint8_t> channels(3,w,h);
#define     SPLIT_SAMPLES(XPS) s3.split(channels,img3,XPS)
            TMX_FOR(SPLIT_SAMPLES);
        }

    }
}
YOCTO_UNIT_TEST_DONE()
