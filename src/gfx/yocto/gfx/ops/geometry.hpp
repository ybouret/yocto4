#ifndef YOCTO_OPS_GEOMETRY_INCLUDED
#define YOCTO_OPS_GEOMETRY_INCLUDED 1

#include "yocto/gfx/pixmap.hpp"
#include "yocto/gfx/xpatch.hpp"
#include "yocto/code/bzset.hpp"

namespace yocto
{
    namespace gfx
    {

        class geometry
        {
        public:
            explicit geometry() throw() : tgt(0), src(0), vtx(), arg(0), brg(0), crg(0) {}
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

            template <typename T>
            void symmetry(pixmap<T>       &target,
                          const pixmap<T> &source,
                          const float      a,
                          const float      b,
                          const float      c,
                          xpatches        &xps)
            {
                tgt = &target;
                src = &source;
                arg = a;
                brg = b;
                crg = c;
                //std::cerr << "a=" << arg << ",b=" << brg << ",c=" << crg << std::endl;
                xps.submit(this, & geometry::__symline<T>);
            }

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(geometry);
            void       *tgt;
            const void *src;
            vertex      vtx;
            float       arg;
            float       brg;
            float       crg;

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
                const point2d<float>      center(float(vtx.x),float(vtx.y));

                for(unit_t y=ymax;y>=ymin;--y)
                {
                    for(unit_t x=xmax;x>=xmin;--x)
                    {
                        const point2d<float>   here(float(x),float(y));
                        const point2d<float>   curr = here-center;
                        const point2d<float>   prev(ca*curr.x+sa*curr.y,-sa*curr.x+ca*curr.y);
                        const vertex  from( unit_t( floorf(center.x+prev.x+0.5f) ), unit_t( floorf(center.y+prev.y+0.5f) ) );
                        if(source.has(from))
                        {
                            target[y][x] = source[from];
                        }
                        else
                        {
                            bzset(target[y][x]);
                        }
                    }
                }

            }


            template <typename T>
            inline void __symline( xpatch &xp, lockable & ) throw()
            {
                assert(tgt);
                assert(src);

                const unit_t     ymin     = xp.lower.y;
                const unit_t     ymax     = xp.upper.y;
                const unit_t     xmin     = xp.lower.x;
                const unit_t     xmax     = xp.upper.x;
                pixmap<T>       &target = *static_cast<pixmap<T> *      >(tgt);
                const pixmap<T> &source = *static_cast<const pixmap<T> *>(src);
                const float      a      = arg;
                const float      b      = brg;
                const float      c      = crg;
                const point      N(a,b);
                const float      den = N.norm2();
                for(unit_t y=ymax;y>=ymin;--y)
                {
                    for(unit_t x=xmax;x>=xmin;--x)
                    {
                        const point   here(float(x),float(y));
                        const float   num = N*here+c;
                        const float   fac = (num+num)/den;
                        const point   sym = here - fac * N;
                        const vertex  from( unit_t(floorf(sym.x+0.5f)), unit_t(floorf(sym.y+0.5f)) );
                        if(source.has(from))
                        {
                            target[y][x] = source[from];
                        }
                        else
                        {
                            bzset(target[y][x]);
                        }
                    }
                }

            }

        };



    }
}
#endif
