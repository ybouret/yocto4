#ifndef YOCTO_GFX_OPS_TRANSFORM_INCLUDED
#define YOCTO_GFX_OPS_TRANSFORM_INCLUDED 1

#include "yocto/gfx/pixmap.hpp"
#include "yocto/gfx/xpatch.hpp"

namespace yocto
{
    namespace gfx
    {

        class transform
        {
        public:
            explicit transform() throw() : tgt(0), src(0), pfn(0), arg(0) {}
            virtual ~transform() throw() {}

            template <typename T,typename U,typename FUNC> inline
            void apply( pixmap<T> &target, FUNC &func, const pixmap<U> &source, xpatches &xps)
            {
                tgt = &target;
                src = &source;
                pfn = (void*)&func;
                arg = 0;
                xps.submit(this, & transform::run0<T,U,FUNC>);
            }

            template <typename T,typename U,typename FUNC,typename ARG> inline
            void apply( pixmap<T> &target, FUNC &func, const pixmap<U> &source, const ARG &params, xpatches &xps)
            {
                tgt = &target;
                src = &source;
                pfn = (void*)&func;
                arg = &params;
                xps.submit(this, & transform::run1<T,U,FUNC,ARG>);
            }

        private:
            void       *tgt;
            const void *src;
            void       *pfn;
            const void *arg;

            template <typename T,typename U,typename FUNC> inline
            void run0( xpatch &xp, lockable &)
            {
                assert(tgt); pixmap<T>       &target = *static_cast< pixmap<T> *    >(tgt);
                assert(src); const pixmap<U> &source = *static_cast<const pixmap<U>*>(src);
                assert(pfn); FUNC            &func   = *(FUNC *)(pfn);
                assert(!arg);

                const unit_t         ymin     = xp.lower.y;
                const unit_t         ymax     = xp.upper.y;
                const unit_t         xmin     = xp.lower.x;
                const unit_t         xmax     = xp.upper.x;

                for(unit_t y=ymax;y>=ymin;--y)
                {
                    typename       pixmap<T>::row &t_y = target[y];
                    const typename pixmap<U>::row &s_y = source[y];
                    
                    for(unit_t x=xmax;x>=xmin;--x)
                    {
                        t_y[x] = func(s_y[x]);
                    }
                }
            }

            template <typename T,typename U,typename FUNC,typename ARG> inline
            void run1( xpatch &xp, lockable &)
            {
                assert(tgt); pixmap<T>       &target = *static_cast< pixmap<T> *    >(tgt);
                assert(src); const pixmap<U> &source = *static_cast<const pixmap<U>*>(src);
                assert(pfn); FUNC            &func   = *(FUNC *)(pfn);
                assert(arg); const ARG       &params = *static_cast<const ARG *>(arg);

                const unit_t         ymin     = xp.lower.y;
                const unit_t         ymax     = xp.upper.y;
                const unit_t         xmin     = xp.lower.x;
                const unit_t         xmax     = xp.upper.x;

                for(unit_t y=ymax;y>=ymin;--y)
                {
                    typename       pixmap<T>::row &t_y = target[y];
                    const typename pixmap<U>::row &s_y = source[y];

                    for(unit_t x=xmax;x>=xmin;--x)
                    {
                        t_y[x] = func(s_y[x],params);
                    }
                }
            }


        };

    }
}

#endif
