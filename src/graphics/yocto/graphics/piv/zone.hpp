#ifndef YOCTO_GRAPHICS_PIV_ZONE_INCLUDED
#define YOCTO_GRAPHICS_PIV_ZONE_INCLUDED 1

#include "yocto/graphics/rawpix.hpp"
#include "yocto/parallel/split.hpp"

namespace yocto
{
    namespace graphics
    {

        namespace PIV
        {

            class Zone
            {
            public:

                static vertex Count( const bitmap &src, const unit_t window_size ) throw()
                {
                    vertex n(3,3);
                    while(n.x*window_size<src.w) ++n.x;
                    --n.x;
                    while(n.y*window_size<src.h) ++n.y;
                    --n.y;
                    return n;
                }

                const vertex r;
                const vertex size;
                const vertex dmin;
                const vertex dmax;

                explicit Zone( const bitmap &src, const vertex indx, const vertex count ) throw() :
                r(),
                size(),
                dmin(),
                dmax()
                {
                    assert(indx.x>=0); assert(indx.x<count.x);
                    assert(indx.y>=0); assert(indx.y<count.y);
                    const unit_t w    = src.w;
                    const unit_t h    = src.h;
                    vertex    & _r    = (vertex &)r;
                    vertex    & _size = (vertex &)size;
                    _r.x = 0;
                    _r.y = 0;
                    _size.x = w;
                    _size.y = h;
                    parallel::basic_split(indx.x, count.x, _r.x, _size.x);
                    parallel::basic_split(indx.y, count.y, _r.y, _size.y);
                    (unit_t&)(dmin.x) = -r.x;
                    (unit_t&)(dmin.y) = -r.y;
                }



                ~Zone() throw() {}


                

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(Zone);
            };
        }
    }
}

#endif

