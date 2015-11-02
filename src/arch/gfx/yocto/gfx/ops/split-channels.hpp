#ifndef YOCTO_GFX_OPS_SPLIT_CHANNELS_INCLUDED
#define YOCTO_GFX_OPS_SPLIT_CHANNELS_INCLUDED 1

#include "yocto/gfx/pixmaps.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace gfx
    {
        
        template <
        typename T,
        typename U,
        template <typename> class COLORSPACE>
        inline void split_channels(pixmaps<T>                    &ch,
                                   const pixmap< COLORSPACE<U> > &px) throw()
        {
            assert(ch.w==px.w);
            assert(ch.h==px.h);
            const size_t n   = min_of<size_t>(ch.size,sizeof(COLORSPACE<U>)/sizeof(U));
            const unit_t w   = ch.w;
            const unit_t h   = ch.h;
            for(unit_t j=0;j<h;++j)
            {
                const typename pixmap< COLORSPACE<U> >::row &src = px[j];
                for(unit_t i=0;i<w;++i)
                {
                    const U *q = (const U *)& src[i];
                    for(size_t c=0;c<n;++c)
                    {
                        ch[c][j][i] = q[c];
                    }
                }
            }
        }

        template <
        typename T,
        typename U,
        template <typename> class COLORSPACE>
        inline void merge_channels( const pixmaps<T> &ch, pixmap< COLORSPACE<U> > &px) throw()
        {
            assert(ch.w==px.w);
            assert(ch.h==px.h);
            const size_t n   = min_of<size_t>(ch.size,sizeof(COLORSPACE<U>)/sizeof(U));
            const unit_t w   = ch.w;
            const unit_t h   = ch.h;
            for(unit_t j=0;j<h;++j)
            {
                typename pixmap< COLORSPACE<U> >::row &tgt = px[j];
                for(unit_t i=0;i<w;++i)
                {
                    U *q = ( U *)& tgt[i];
                    for(size_t c=0;c<n;++c)
                    {
                        q[c] = ch[c][j][i];
                    }
                }

            }
        }

    }
}


#endif
