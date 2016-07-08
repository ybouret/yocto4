#ifndef YOCTO_GRAPHICS_OPS_MEDIAN_INCLUDED
#define YOCTO_GRAPHICS_OPS_MEDIAN_INCLUDED 1

#include "yocto/graphics/xpatch.hpp"
#include "yocto/graphics/rawpix.hpp"

namespace yocto
{
    namespace graphics
    {


        class median
        {
        public:
            inline explicit median() throw() : tgt(0), src(0) {}
            inline virtual ~median() throw() {}

            template <typename T>
            void filter(pixmap<T>         &target,
                        const pixmap<T>   &source,
                        xpatches          &xps,
                        threading::engine *server)
            {
                tgt = &target;
                src = &source;
                const size_t np = xps.size();
                for(size_t i=np;i>0;--i)
                {
                    xpatch &xp = xps[i];
                    xp.enqueue(this, & median::apply<T>, server);
                }
                if(server) server->flush();
            }


        private:
            void       *tgt;
            const void *src;

            template <typename T>
            inline void apply( xpatch &xp, lockable & ) throw()
            {
                //assert(tgt); pixmap<T>       &target = *static_cast< pixmap<T> *      >(tgt);
                assert(src); const pixmap<T> &source = *static_cast< const pixmap<T>* >(src);
                const unit_t ymin = xp.lower.y;
                const unit_t ymax = xp.upper.y;

                const unit_t xmin = xp.lower.x;
                const unit_t xmax = xp.upper.x;

                T arr[9];
                vertex v;
                for(v.y=ymax;v.y>=ymin;--v.y)
                {
                    for(v.x=xmax;v.x>=xmin;--v.x)
                    {
                        size_t count = 0;
                        arr[count++] = source[v];
                        for(size_t k=0;k<8;++k)
                        {
                            const vertex probe = v+gist::delta[k];
                            if(source.has(probe))
                            {
                                arr[count++] = source[probe];
                            }
                        }
                    }
                }


            }

        };

    }
}

#endif
