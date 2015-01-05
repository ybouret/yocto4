#ifndef YOCTO_GFX_RAWPIX_MC_INCLUDED
#define YOCTO_GFX_RAWPIX_MC_INCLUDED 1

#include "yocto/gfx/rawpix.hpp"
#include "yocto/code/bzset.hpp"

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
                    if(hi<tmp)
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
        
        
        template <typename T, typename U>
        inline void maximum_contrast( pixmap<T> &target, const pixmap<U> &source ) throw()
        {
            assert(source.w==target.w);
            assert(source.h==target.h);
            
            
            // Get the source limits
            U lo = source[0][0];
            U hi = lo;
            
            const unit_t w = source.w;
            const unit_t h = source.h;
            for(unit_t j=0;j<h;++j)
            {
                typename pixmap<T>::row       &Tj = target[j];
                const typename pixmap<U>::row &Sj = source[j];
                for(unit_t i=0;i<w;++i)
                {
                    bzset(Tj[i]);
                    const U tmp = Sj[i];
                    if(tmp<lo)
                    {
                        lo=tmp;
                    }
                    if(hi<tmp)
                    {
                        hi=tmp;
                    }
                }
            }
            
            // Rescale
            if(lo<hi)
            {
                static const double fac = opaque<T>::value;
                static const double den = double(hi-lo);
                for(unit_t j=0;j<h;++j)
                {
                    typename pixmap<T>::row       &Tj = target[j];
                    const typename pixmap<U>::row &Sj = source[j];
                    for(unit_t i=0;i<w;++i)
                    {
                        const U s = Sj[i];
                        const double num = fac * double(s-lo);
                        Tj[i] = T(num/den);
                    }
                }
            }

            
        }
        
    }
}

#endif
