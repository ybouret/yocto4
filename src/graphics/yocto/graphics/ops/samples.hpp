#ifndef YOCTO_GRAPHICS_OPS_SAMPLES_INCLUDED
#define YOCTO_GRAPHICS_OPS_SAMPLES_INCLUDED 1

#include "yocto/graphics/xpatch.hpp"
#include "yocto/graphics/pixmaps.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace graphics
    {
        class samples
        {
        public:
            inline explicit samples() throw() {}
            inline virtual ~samples() throw() {}

            struct io_data
            {
                size_t      nch;
                void       *tgt;
                const void *src;
            };

            template <typename T,typename RGB_TYPE>
            inline void split(pixmaps<T>             &tgt,
                              const pixmap<RGB_TYPE> &src,
                              xpatches               &xps,
                              threading::engine      *server)
            {
                const size_t nch = min_of<size_t>(tgt.size,sizeof(RGB_TYPE)/sizeof(T));
                for(size_t i=xps.size();i>0;--i)
                {
                    xpatch  &xp   = xps[i];
                    io_data &data = xp.make<io_data>();
                    data.nch = nch;
                    data.src = &src;
                    data.tgt = &tgt;
                    xp.enqueue(this, &samples::split_cb<T,RGB_TYPE>,server);
                }

            }

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(samples);

            template <typename T,typename RGB_TYPE>
            inline void split_cb( xpatch &xp, lockable & )
            {
                io_data                &data  = xp.as<io_data>();
                pixmaps<T>             &tgt   = *static_cast<pixmaps<T> *>(data.tgt);
                const pixmap<RGB_TYPE> &src   = *static_cast<const pixmap<RGB_TYPE> *>(data.src);
                const size_t nch   = data.nch;
                const vertex lower = xp.lower;
                const vertex upper = xp.upper;
                for(unit_t j=upper.y;j>=lower.y;--j)
                {
                    const typename pixmap<RGB_TYPE>::row &s_j = src[j];
                    for(unit_t i=upper.x;i>=lower.x;--i)
                    {
                        const RGB_TYPE C = s_j[i];
                        const typename RGB_TYPE::type *p = (const typename RGB_TYPE::type *)&C;

                        for(size_t k=0;k<nch;++k)
                        {
                            tgt[k][j][i] = p[k];
                        }
                    }
                }
                
            }
        };
    }
}

#endif
