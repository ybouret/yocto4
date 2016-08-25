#include "yocto/gfx/image.hpp"
#include "yocto/gfx/image/jpeg.hpp"
#include "yocto/gfx/image/png.hpp"
#include "yocto/gfx/image/tiff.hpp"

#include "yocto/gfx/ops/edges.hpp"
#include "yocto/gfx/color/named-colors.hpp"
#include "yocto/gfx/draw/circle.hpp"

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
        return RGBA(0,0,0,0);
    }


private:
    YOCTO_DISABLE_COPY_AND_ASSIGN(dir2col);
};

YOCTO_UNIT_TEST_IMPL(edges)
{

    YOCTO_GFX_DECL_FORMAT(jpeg);
    YOCTO_GFX_DECL_FORMAT(png);
    YOCTO_GFX_DECL_FORMAT(tiff);

    dir2col rmp;

    imageIO          &IMG = image::instance();
    if(argc>1)
    {
        std::cerr << "-- Load" << std::endl;
        const string  filename = argv[1];
        pixmapf       img( IMG.loadf(filename,NULL) );




        const unit_t w=img.w;
        const unit_t h=img.h;

        if(true)
        {
            img.ldz();
            draw_circle(img, w/2, h/2, min_of(w,h)/4, 1.0f);
        }

        IMG.save("img-f.png",img,NULL);
        xpatches xps(img,true);

        EdgeDetector        ED(w,h);

        float sig = 1.4f;
        if(argc>2)
        {
            sig = strconv::to<float>(argv[2],"sig");
        }

        pixmapf img3(w,h);
        pixmapf img5(w,h);
        {
            const stencil_gauss   g3(1,sig);
            const stencil_gauss   g5(2,sig);
            stencil::dispatcher   dsp(w,h);
            dsp(g3,img3,img,xps);
            dsp(g5,img5,img,xps);
        }
        IMG.save("img-3.png",img3,NULL);
        IMG.save("img-5.png",img5,NULL);

        const stencil_grad_x  gx;
        const stencil_grad_y  gy;

        const stencil_sobel_x sx;
        const stencil_sobel_y sy;

        const stencil_scharr_x Sx;
        const stencil_scharr_y Sy;

        ED.build_from(img, Sx, Sy, xps);
        IMG.save("img-grad-f.png",ED,0);
        IMG.save("img-angl-f.png",ED.A,rmp,0);

        ED.build_from(img3, Sx, Sy, xps);
        IMG.save("img-grad-3.png",ED,0);
        IMG.save("img-angl-3.png",ED.A,rmp,0);


        ED.build_from(img5, Sx, Sy, xps);
        IMG.save("img-grad-5.png",ED,0);
        IMG.save("img-angl-5.png",ED.A,rmp,0);





    }

}
YOCTO_UNIT_TEST_DONE()

