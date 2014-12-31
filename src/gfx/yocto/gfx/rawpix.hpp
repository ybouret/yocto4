#ifndef YOCTO_GFX_RAWPIX_INCLUDED
#define YOCTO_GFX_RAWPIX_INCLUDED 1

#include "yocto/gfx/pixmap.hpp"
#include "yocto/gfx/rgb.hpp"

namespace yocto
{
    namespace gfx
    {
        
        typedef pixmap<rgb_t>  pixmap3;
        typedef pixmap<rgba_t> pixmap4;
        typedef pixmap<float>  pixmapf;
        typedef pixmap<double> pixmapd;
        
        template <typename T>
        inline void maximum_contrast( pixmap<T> &pxm ) throw()
        {
            T lo = pxm[0][0];
            T hi = lo;
            const unit_t w = pxm.w;
            const unit_t h = pxm.h;
            for(unit_t j=0;j<h;++j)
            {
                const typename pixmap<T>::row &Rj = pxm[j];
                for(unit_t i=0;i<w;++i)
                {
                    const T tmp = Rj[i];
                    if(tmp<lo)
                    {
                        lo=tmp;
                    }
                    if(tmp>hi)
                    {
                        hi=tmp;
                    }
                }
            }
            if(lo<hi)
            {
                const T den = hi-lo;
                for(unit_t j=0;j<h;++j)
                {
                    typename pixmap<T>::row &Rj = pxm[j];
                    for(unit_t i=0;i<w;++i)
                    {
                        T &tmp = Rj[i];
                        tmp = (tmp-lo)/den;
                    }
                }
            }
        }
        
        
    }
}

#endif
