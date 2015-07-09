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
                //typename pixmap<T>::row       &Tj = target[j];
                const typename pixmap<U>::row &Sj = source[j];
                for(unit_t i=0;i<w;++i)
                {
                    //bzset(Tj[i]);
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
        
        template <typename T,typename U,size_t N>
        inline void enhance( bitmap &target, const bitmap &source ) throw()
        {
            assert(source.w==target.w);
            assert(source.h==target.h);
            assert(source.d>=sizeof(U)*N);
            assert(target.d>=sizeof(T)*N);
            
            // Get the source limits
            U lo[N];
            U hi[N];
            const U *u = (const U *)source.get_line(0);
            for(size_t k=0;k<N;++k)
            {
                lo[k] = hi[k] = u[k];
            }
            
            const unit_t h = source.h;
            const unit_t w = source.w;
            const unit_t s = source.d;
            for(unit_t j=0;j<h;++j)
            {
                const uint8_t *p = (const uint8_t *)source.get_line(j);
                for(unit_t i=0;i<w;++i,p += s)
                {
                    const U *u = (const U*)p;
                    for(size_t k=0;k<N;++k)
                    {
                        const U tmp = u[k];
                        if(tmp<lo[k]) { lo[k] = tmp; }
                        if(tmp>hi[k]) { hi[k] = tmp; }
                    }
                    
                }
            }
            
            for(size_t k=0;k<N;++k)
            {
                std::cerr << "channel" << k << ": " << double(lo[k]) << "->" << double(hi[k]) << std::endl;
            }
            
            // Get the scaling factors
            static const double fac = opaque<T>::value;
            double den[N];
            bool   ok[N];
            for(size_t k=0;k<N;++k)
            {
                den[k] = hi[k] - u[k];
                ok[k]  = den[k]>0;
            }
            const unit_t t = target.d;
            for(unit_t j=0;j<h;++j)
            {
                const uint8_t *p = (const uint8_t *)source.get_line(j);
                uint8_t       *q = (uint8_t       *)target.get_line(j);
                for(unit_t i=0;i<w;++i,p += s, q += t)
                {
                    const U *src = (const U*)p;
                    T       *tgt = (T      *)q;
                    for(size_t k=0;k<N;++k)
                    {
                        if(ok[k])
                        {
                            const double num = fac * double(src[k]-lo[k]);
                            tgt[k] = T(num/den[k]);
                        }
                        
                    }
                    
                }
                
            }
            
            
        }
        
        
    }
}

#endif
