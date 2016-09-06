#include "yocto/gfx/image.hpp"
#include "yocto/gfx/image/jpeg.hpp"
#include "yocto/gfx/image/png.hpp"
#include "yocto/gfx/image/tiff.hpp"

#include "yocto/gfx/ops/edges.hpp"
#include "yocto/gfx/color/named-colors.hpp"
#include "yocto/gfx/draw/circle.hpp"
#include "yocto/gfx/color/ramp/cold_to_very_hot.hpp"
#include "yocto/gfx/color/ramp/cold_to_hot.hpp"
#include "yocto/gfx/color/ramp/grey.hpp"

#include "yocto/code/rand.hpp"
#include "yocto/string/conv.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace gfx;
using namespace math;


class dir2col : public data2rgba
{
public:
    inline dir2col()
    {
    }

    virtual ~dir2col() throw()
    {
    }

    inline virtual RGBA get(const void *addr) throw()
    {
        const size_t indx = *(const uint8_t *)addr;
        switch(indx)
        {
            case 1:
                return named_color::fetch(YGFX_RED);

            case 3:
                return named_color::fetch(YGFX_CYAN);

            case 2:
                return named_color::fetch(YGFX_YELLOW);

            case 4:
                return named_color::fetch(YGFX_GREEN);

            default:
                break;
        }
        return RGBA(0,0,0);
    }


private:
    YOCTO_DISABLE_COPY_AND_ASSIGN(dir2col);
};



YOCTO_UNIT_TEST_IMPL(edges)
{

    YOCTO_GFX_DECL_FORMAT(jpeg);
    YOCTO_GFX_DECL_FORMAT(png);
    YOCTO_GFX_DECL_FORMAT(tiff);

    dir2col          rmp;
    cold_to_hot      crmp;
    grey             grmp;

    crmp.vmin = 0;
    crmp.vmax = 1;

    grmp.vmin = 0;
    grmp.vmax = 0;


    imageIO          &IMG = image::instance();
    if(argc>1)
    {
        std::cerr << "-- Load" << std::endl;
        const string  filename = argv[1];
        pixmapf       img( IMG.loadf(filename,NULL) );




        const unit_t w=img.w;
        const unit_t h=img.h;

        if(false)
        {
            img.ldz();
            //draw_circle(img, w/2, h/2, min_of(w,h)/4, 1.0f);
            draw_disk(img, w/2, h/2, min_of(w,h)/4, 1.0f);
        }

        xpatches xps(img,true);

        EdgeDetector        ED(w,h);

        float sig = 1.4f;
        if(argc>2)
        {
            sig = strconv::to<float>(argv[2],"sig");
        }

        pixmapf img3(w,h);
        pixmapf img5(w,h);
        pixmapf img7(w,h);
        {
            const stencil_gauss   g3(1,sig);
            const stencil_gauss   g5(2,sig);
            const stencil_gauss   g7(3,sig);
            stencil::dispatcher   dsp(w,h);
            std::cerr << "Gauss3x3,sigma=" << sig << std::endl;
            dsp(g3,img3,img,xps);
            std::cerr << "Gauss5x5,sigma=" << sig << std::endl;
            dsp(g5,img5,img,xps);
            std::cerr << "Gauss7x7,sigma=" << sig << std::endl;
            dsp(g7,img7,img,xps);

        }
        IMG.save("img-0.png",img,NULL);
        IMG.save("img-3.png",img3,NULL);
        IMG.save("img-5.png",img5,NULL);
        IMG.save("img-7.png",img7,NULL);


        const stencil_scharr_x5 Sx;
        const stencil_scharr_y5 Sy;

        ED.build_from(img, Sx, Sy, xps);
        IMG.save("img-grad-0.png",ED,grmp,0);
        IMG.save("img-angl-0.png",ED.A,rmp,0);
        IMG.save("img-nmax-0.png",ED.E,0);
        IMG.save("img-blob-0.png",ED.tags,ED.tags.colors,0);

        ED.build_from(img3, Sx, Sy, xps);
        IMG.save("img-grad-3.png",ED,grmp,0);
        IMG.save("img-angl-3.png",ED.A,rmp,0);
        IMG.save("img-nmax-3.png",ED.E,0);
        IMG.save("img-blob-3.png",ED.tags,ED.tags.colors,0);


        ED.build_from(img5, Sx, Sy, xps);
        IMG.save("img-grad-5.png",ED,grmp,0);
        IMG.save("img-angl-5.png",ED.A,rmp,0);
        IMG.save("img-nmax-5.png",ED.E,0);
        IMG.save("img-blob-5.png",ED.tags,ED.tags.colors,0);

        ED.build_from(img7, Sx, Sy, xps);
        IMG.save("img-grad-7.png",ED,grmp,0);
        IMG.save("img-angl-7.png",ED.A,rmp,0);
        IMG.save("img-nmax-7.png",ED.E,0);
        IMG.save("img-blob-7.png",ED.tags,ED.tags.colors,0);




    }

}
YOCTO_UNIT_TEST_DONE()

