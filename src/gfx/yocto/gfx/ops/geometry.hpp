#ifndef YOCTO_OPS_GEOMETRY_INCLUDED
#define YOCTO_OPS_GEOMETRY_INCLUDED 1

#include "yocto/gfx/pixmap.hpp"
#include "yocto/gfx/xpatch.hpp"

namespace yocto
{
    namespace gfx
    {

        class geometry
        {
        public:
            explicit geometry() throw() : tgt(0), src(0), vtx(), arg(0) {}
            virtual ~geometry() throw() {}

            template <typename T>
            void rotate(pixmap<T>       &target,
                        const pixmap<T> &source,
                        const vertex     center,
                        const float      angle,
                        xpatches        &xps)
            {
                tgt = &target;
                src = &source;
                vtx = center;
                arg = angle;
                xps.submit(this, & geometry::__rotate<T>);
            }

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(geometry);
            void       *tgt;
            const void *src;
            vertex      vtx;
            float       arg;

            template <typename T>
            inline void __rotate( xpatch &xp, lockable & ) throw()
            {
                assert(tgt);
                assert(src);

                const unit_t         ymin     = xp.lower.y;
                const unit_t         ymax     = xp.upper.y;
                const unit_t         xmin     = xp.lower.x;
                const unit_t         xmax     = xp.upper.x;
                pixmap<T>       &target = *static_cast<pixmap<T> *      >(tgt);
                const pixmap<T> &source = *static_cast<const pixmap<T> *>(src);
                const float      ca     = cosf(arg);
                const float      sa     = sinf(arg);
                const point      center(vtx.x,vtx.y);

                for(unit_t y=ymax;y>=ymin;--y)
                {
                    for(unit_t x=xmax;x>=xmin;--x)
                    {
                        const point   here(x,y);
                        const point   curr = here-center;
                        const point   prev(ca*curr.x+sa*curr.y,-sa*curr.x+ca*curr.y);
                        const vertex  from( unit_t( floorf(center.x+prev.x+0.5f) ), unit_t( floorf(center.y+prev.y+0.5f) ) );
                        if(source.has(from))
                        {
                            target[y][x] = source[from];
                        }
                    }
                }

            }

        };

    }
}
#endif
