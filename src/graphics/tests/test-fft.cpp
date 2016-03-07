#include "yocto/utest/run.hpp"
#include "yocto/graphics/ops/fft.hpp"
#include "yocto/graphics/image/png.hpp"
#include "yocto/graphics/image/jpeg.hpp"
#include "yocto/graphics/image/tiff.hpp"
#include "yocto/code/utils.hpp"

using namespace yocto;
using namespace graphics;

YOCTO_UNIT_TEST_IMPL(fft)
{
    image &IMG = image::instance();

    IMG.declare( new png_format()  );
    IMG.declare( new jpeg_format() );
    IMG.declare( new tiff_format() );


    //const image::format &PNG = IMG["PNG"];
    const imageIO       &gfx = IMG;

    if(argc>1)
    {
        const string filename = argv[1];

        // load
        std::cerr << "Loading FLOAT" << std::endl;
        pixmapf pf( IMG.loadf(filename, NULL));

        gfx.save("gs.png", pf, NULL);

        const unit_t w = pf.w;
        const unit_t h = pf.h;

        const unit_t aw = next_power_of_two(w);
        const unit_t ah = next_power_of_two(h);
        pixmapz      pz(aw,ah);
        for(unit_t j=0;j<h;++j)
        {
            for(unit_t i=0;i<w;++i)
            {
                pz[j][i].re = pf[j][i];
            }

        }

        std::cerr << "FFT on " << pz.w << "x" << pz.h << std::endl;
        fft::forward(pz);

        pixmapf pA(aw,ah);
        float amax = 0.0f;
        for(unit_t j=0;j<ah;++j)
        {
            for(unit_t i=0;i<aw;++i)
            {
                const float a = pz[j][i].mod();
                pA[j][i] = a;
                amax     = max_of(a,amax);
            }
        }

        std::cerr << "amax=" << amax << std::endl;
        if(amax>0)
        {
            for(unit_t j=0;j<ah;++j)
            {
                for(unit_t i=0;i<aw;++i)
                {
                    pA[j][i] /= amax;
                }
            }
        }

        gfx.save("fft.png", pA, NULL);


        const float sig = min_of(w,h)/4.0f;
        const float sig2= 2*sig*sig;
        for(unit_t j=0;j<ah;++j)
        {
            const float j2 = (j*j);
            for(unit_t i=0;i<aw;++i)
            {
                const float i2 = (i*i);
                const float r2 = i2+j2;
                const float fac = expf(-r2/sig2);
                pz[j][i] *= fac;
            }
        }

        fft::reverse(pz);

        pixmapf tgt(w,h);
        for(unit_t j=0;j<h;++j)
        {
            for(unit_t i=0;i<w;++i)
            {
                tgt[j][i] = pz[j][i].re;
            }
        }
        gfx.save("tgt.png", tgt, NULL);


    }
}
YOCTO_UNIT_TEST_DONE()


