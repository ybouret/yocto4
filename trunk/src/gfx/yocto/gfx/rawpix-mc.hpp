#ifndef YOCTO_GFX_RAWPIX_MC_INCLUDED
#define YOCTO_GFX_RAWPIX_MC_INCLUDED 1

#include "yocto/gfx/rawpix.hpp"

namespace yocto
{
    
    namespace gfx
    {
        
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
                static const double fac = opaque<T>::value;
                const        T      den = hi-lo;
                for(unit_t j=0;j<h;++j)
                {
                    typename pixmap<T>::row &Rj = pxm[j];
                    for(unit_t i=0;i<w;++i)
                    {
                        T &tmp = Rj[i];
                        const double num = fac * double(tmp-lo);
                        tmp = T(num/den);
                    }
                }
            }
        }
        
    }
}

#endif
