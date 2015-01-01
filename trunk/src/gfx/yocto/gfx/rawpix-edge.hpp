#ifndef YOCTO_GFX_RAWPIX_EDGE_INCLUDED
#define YOCTO_GFX_RAWPIX_EDGE_INCLUDED 1

#include "yocto/gfx/rawpix.hpp"
#include "yocto/code/bzset.hpp"
#include "yocto/math/types.hpp"

namespace yocto
{
    
    namespace gfx
    {
        
        template <typename T, typename U>
        inline void compute_edge( pixmap<T> &tgt, const pixmap<U> &src ) throw()
        {
            assert(tgt.w==src.w);
            assert(tgt.h==src.h);
            const unit_t w1 = src.w-1;
            const unit_t h1 = src.h-1;
            
            // set border to 0
            {
                typename pixmap<T>::row &r_ini = tgt[ 0];
                typename pixmap<T>::row &r_end = tgt[h1];
                for(unit_t i=0;i<=w1;++i)
                {
                    bzset(r_ini[i]);
                    bzset(r_end[i]);
                }
            }
            
            for(unit_t j=0;j<=h1;++j)
            {
                bzset(tgt[j][0]);
                bzset(tgt[j][w1]);
            }
            
            // go inside
            T gmax = 0;
            for(unit_t j=1,jm=0,jp=2;j<h1;++j,++jm,++jp)
            {
                typename       pixmap<T>::row &tgt_j  = tgt[j];
                const typename pixmap<U>::row &src_j  = src[j];
                const typename pixmap<U>::row &src_jp = src[jp];
                const typename pixmap<U>::row &src_jm = src[jm];
                
                for(unit_t i=1,im=0,ip=2;i<w1;++i,++im,++ip)
                {
                    const T gx = T(src_j[ip]) - T(src_j[im]);
                    const T gy = T(src_jp[i]) - T(src_jm[i]);
                    const T g  = math::Hypotenuse(gx,gy);
                    tgt_j[i]   = g;
                    if(g>gmax)
                    {
                        gmax = g;
                    }
                }
            }
            
            if(gmax>0)
            {
                for(unit_t j=1;j<h1;++j)
                {
                    typename pixmap<T>::row &tgt_j  = tgt[j];
                    for(unit_t i=1;i<w1;++i)
                    {
                        tgt_j[i] /= gmax;
                    }
                }
            }
            
        }
    }
    
}

#endif
