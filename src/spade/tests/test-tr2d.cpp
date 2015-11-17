#include "yocto/spade/geometry/point-in-triangle2d.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/graphics/image/png.hpp"
#include "yocto/graphics/rawpix.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/graphics/named-colors.hpp"

using namespace yocto;
using namespace spade;

YOCTO_UNIT_TEST_IMPL(tr2d)
{
    graphics::image      &IMG = graphics::image::instance();
    graphics::png_format *PNG = new graphics::png_format();
    IMG.declare(PNG);

    graphics::pixmap3 img(200,100);



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

        const graphics::RGB c = graphics::named_color::fetch(iter);
        for(unit_t j=0;j<h;++j)
        {
            for(unit_t i=0;i<h;++i)
            {
                const vertex p(i,j);
                if( point_in_triangle2d::check(p, tr[0], tr[1], tr[2], 0.001) )
                {
                    img[j][i] = c;
                }
            }
        }

    }

    PNG->yocto::graphics::imageIO::save("triangle.png", img, NULL);



}
YOCTO_UNIT_TEST_DONE()
