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
            inline explicit samples() throw() : _nch(0), _tgt(0), _src(0) {}
            inline virtual ~samples() throw() {}

            template <typename T,typename RGB_TYPE>
            inline void split(pixmaps<T>             &tgt,
                              const pixmap<RGB_TYPE> &src,
                              xpatches               &xps,
                              threading::engine      *server)
            {
                _nch = min_of<size_t>(tgt.size,sizeof(RGB_TYPE)/sizeof(T));
                _src = &src;
                _tgt = &tgt;
                for(size_t i=xps.size();i>0;--i)
                {
                    xpatch  &xp   = xps[i];
                    xp.enqueue(this, &samples::split_cb<T,RGB_TYPE>,server);
                }
                if(server) server->flush();
            }

            template <typename T,typename RGB_TYPE>
            inline void merge(const pixmaps<T>             &src,
                              pixmap<RGB_TYPE>             &tgt,
                              xpatches                     &xps,
                              threading::engine            *server)
            {
                _nch = min_of<size_t>(src.size,sizeof(RGB_TYPE)/sizeof(T));
                _src = &src;
                _tgt = &tgt;
                for(size_t i=xps.size();i>0;--i)
                {
                    xpatch  &xp   = xps[i];
                    xp.enqueue(this, &samples::merge_cb<T,RGB_TYPE>,server);
                }
                if(server) server->flush();
            }


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(samples);
            size_t       _nch;
            void  *      _tgt;
            const void * _src;
            
            template <typename T,typename RGB_TYPE>
            inline void split_cb( xpatch &xp, lockable & )
            {
                pixmaps<T>             &tgt   = *static_cast<pixmaps<T> *>(_tgt);
                const pixmap<RGB_TYPE> &src   = *static_cast<const pixmap<RGB_TYPE> *>(_src);
                const size_t            nch   = _nch;
                const vertex            lower = xp.lower;
                const vertex            upper = xp.upper;

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

            template <typename T,typename RGB_TYPE>
            inline void merge_cb( xpatch &xp, lockable & )
            {
                const pixmaps<T> &src   = *static_cast<const pixmaps<T> *>(_src);
                pixmap<RGB_TYPE> &tgt   = *static_cast<pixmap<RGB_TYPE> *>(_tgt);
                const size_t      nch   = _nch;
                const vertex      lower = xp.lower;
                const vertex      upper = xp.upper;

                for(unit_t j=upper.y;j>=lower.y;--j)
                {
                    typename pixmap<RGB_TYPE>::row &t_j = tgt[j];
                    for(unit_t i=upper.x;i>=lower.x;--i)
                    {
                        RGB_TYPE &C = t_j[i];
                        typename RGB_TYPE::type *p = (typename RGB_TYPE::type *)&C;

                        for(size_t k=0;k<nch;++k)
                        {
                            p[k] = src[k][j][i];
                        }
                    }
                }

            }

        };
    }
}

#endif
