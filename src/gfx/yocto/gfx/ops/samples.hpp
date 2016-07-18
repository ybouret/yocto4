#ifndef YOCTO_GFX_OPS_SAMPLES_INCLUDED
#define YOCTO_GFX_OPS_SAMPLES_INCLUDED 1

#include "yocto/gfx/pixmaps.hpp"
#include "yocto/gfx/xpatch.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace gfx
    {

        template <typename COLOR_TYPE>
        class samples
        {
        public:
            typedef typename COLOR_TYPE::type T;
            static const size_t max_channels = sizeof(COLOR_TYPE)/sizeof(T);

            inline explicit  samples() throw() : nch(0), tgt(0), src(0) {}
            inline virtual  ~samples() throw() {}
            
            inline void split(pixmaps<T>               &channels,
                              const pixmap<COLOR_TYPE> &source,
                              xpatches                 &xps,
                              threading::engine        *server
                              )
            {
                nch = min_of(channels.size,max_channels);
                tgt = &channels;
                src = &source;
                for(size_t i=xps.size();i>0;--i)
                {
                    xpatch &xp = xps[i];
                    xp.enqueue(this,&samples<COLOR_TYPE>::__split,server);
                }
                if(server) server->flush();
            }


            inline void merge(const pixmaps<T>         &channels,
                              pixmap<COLOR_TYPE>       &target,
                              xpatches                 &xps,
                              threading::engine        *server
                              )
            {
                nch = min_of(channels.size,max_channels);
                src = &channels;
                tgt = &target;
                for(size_t i=xps.size();i>0;--i)
                {
                    xpatch &xp = xps[i];
                    xp.enqueue(this,&samples<COLOR_TYPE>::__merge,server);
                }
                if(server) server->flush();
            }


        private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(samples);
            size_t      nch;
            void       *tgt;
            const void *src;

            inline void __split( xpatch &xp, lockable & ) throw()
            {
                assert(tgt);
                assert(src);
                pixmaps<T>               &channels = *static_cast< pixmaps<T> *>(tgt);
                const pixmap<COLOR_TYPE> &source   = *static_cast< const pixmap<COLOR_TYPE> *>(src);
                assert(min_of(channels.size,max_channels)==nch);

                const unit_t ymin = xp.lower.y;
                const unit_t ymax = xp.upper.y;

                const unit_t xmin = xp.lower.x;
                const unit_t xmax = xp.upper.x;

                for(unit_t y=ymax;y>=ymin;--y)
                {

                    for(unit_t x=xmax;x>=xmin;--x)
                    {
                        const COLOR_TYPE &C = source[y][x]; //!< get source
                        const T          *p = (const T*)&C; //!< first item
                        for(size_t i=0;i<nch;++i)
                        {
                            channels[i][y][x] = p[i];
                        }
                    }
                }
            }


            inline void __merge( xpatch &xp, lockable & ) throw()
            {
                assert(tgt);
                assert(src);
                const pixmaps<T>   &channels = *static_cast< const pixmaps<T>   *>(src);
                pixmap<COLOR_TYPE> &target   = *static_cast< pixmap<COLOR_TYPE> *>(tgt);
                assert(min_of(channels.size,max_channels)==nch);

                const unit_t ymin = xp.lower.y;
                const unit_t ymax = xp.upper.y;

                const unit_t xmin = xp.lower.x;
                const unit_t xmax = xp.upper.x;

                for(unit_t y=ymax;y>=ymin;--y)
                {
                    for(unit_t x=xmax;x>=xmin;--x)
                    {
                        T *p = (T*)&target[y][x];
                        for(size_t i=0;i<nch;++i)
                        {
                            p[i] = channels[i][y][x];
                        }
                    }
                }
            }

        };


    }

}

#endif
