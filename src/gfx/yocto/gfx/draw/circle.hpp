#ifndef YOCTO_GFX_DRAW_CIRCLE_INCLUDED
#define YOCTO_GFX_DRAW_CIRCLE_INCLUDED 1

#include "yocto/gfx/pixmap.hpp"
#include "yocto/gfx/pixel.hpp"

namespace yocto
{
    namespace gfx
    {
        template <typename T>
        inline void draw_circle(pixmap<T>    &img,
                                const unit_t  xm,
                                const unit_t  ym,
                                unit_t        r,
                                const T       C,
                                const uint8_t alpha=0xff) throw()
        {
            assert(r>=0);
            unit_t x = -r, y = 0, err = 2-2*r; /* bottom left to top right */
            do {
                const vertex v[4] = {
                    vertex(xm-x,ym+y),  /*   I. Quadrant +x +y */
                    vertex(xm-y,ym-x), /*  II. Quadrant -x +y */
                    vertex(xm+x,ym-y), /* III. Quadrant -x -y */
                    vertex(xm+y,ym+x)  /*  IV. Quadrant +x -y */
                };
                for(size_t i=0;i<4;++i)
                {
                    const vertex p = v[i];
                    if(img.has(p))
                    {
                        img[p] = pixel<T>::blend(img[p],C,alpha);
                    }
                }
                r = err;
                if (r <= y) err += ++y*2+1;            /* e_xy+e_y < 0 */
                if (r > x || err > y)                  /* e_xy+e_x > 0 or no 2nd y-step */
                    err += ++x*2+1;                    /* -> x-step now */
            } while (x < 0);
        }

        template <typename T>
        inline void draw_hseg(pixmap<T>    &img,
                              const unit_t  y,
                              const unit_t  xlo,
                              const unit_t  xhi,
                              const T       C,
                              const uint8_t alpha) throw()
        {
            assert(xlo<=xhi);
            vertex p(xlo,y);
            for(;p.x<=xhi;++p.x)
            {
                if(img.has(p))
                {
                    img[p] = pixel<T>::blend(img[p],C,alpha);
                }
            }
        }

        template <typename T>
        inline void draw_disk(pixmap<T>      &img,
                                const unit_t  xm,
                                const unit_t  ym,
                                unit_t        r,
                                const T       C,
                                const uint8_t alpha=0xff) throw()
        {
            assert(r>=0);
            unit_t x = -r, y = 0, err = 2-2*r; /* bottom left to top right */
            bool new_y = true;
            do {
                if(new_y)
                {
                    draw_hseg(img,ym-y,xm+x,xm-x,C,alpha);
                    if(y>0)
                    {
                        draw_hseg(img,ym+y,xm+x,xm-x,C,alpha);
                    }
                    new_y = false;
                }
                r = err;
                if (r <= y)
                {
                    err += ++y*2+1;            /* e_xy+e_y < 0 */
                    new_y = true;
                }
                if (r > x || err > y)                  /* e_xy+e_x > 0 or no 2nd y-step */
                    err += ++x*2+1;                    /* -> x-step now */
            } while (x < 0);
        }

    }
}

#endif