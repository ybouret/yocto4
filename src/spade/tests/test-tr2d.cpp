#include "yocto/spade/geometry/point-in-triangle2d.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/gfx/image/png.hpp"
#include "yocto/gfx/rawpix.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/gfx/color/named-colors.hpp"
#include "yocto/gfx/pixel.hpp"

using namespace yocto;
using namespace spade;

YOCTO_UNIT_TEST_IMPL(tr2d)
{
    gfx::image      &IMG = gfx::image::instance();
    gfx::png_format *PNG = new gfx::png_format();
    IMG.declare(PNG);
    gfx::imageIO     *png = PNG;

    gfx::pixmap3 img(200,100);



    const unit_t w = img.w;
    const unit_t h = img.h;
    typedef point2d<float> vertex;

    for(size_t iter=0;iter<3;++iter)
    {
        vertex tr[3];
        for(size_t i=0;i<3;++i)
        {
            tr[i].x = alea_lt(w);
            tr[i].y = alea_lt(h);
        }

        const gfx::RGB c = gfx::named_color::fetch(iter);
        for(unit_t j=0;j<h;++j)
        {
            for(unit_t i=0;i<w;++i)
            {
                const vertex p(i,j);
                if( point_in_triangle2d::check(p, tr[0], tr[1], tr[2], 0.001) )
                {
                    img[j][i] = gfx::pixel<gfx::RGB>::blend(img[j][i], c, 128);
                }
            }
        }

    }

    png->save("triangle.png", img, NULL);



}
YOCTO_UNIT_TEST_DONE()
