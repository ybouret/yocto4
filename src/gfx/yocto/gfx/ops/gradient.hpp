#ifndef YOCTO_GFX_GRADIENT_INCLUDED
#define YOCTO_GFX_GRADIENT_INCLUDED 1

#include "yocto/gfx/pixmaps.hpp"
#include "yocto/math/types.hpp"

namespace yocto
{
    namespace gfx
    {
        
        
        template <typename T>
        double gradient_of( const pixmap<T> &data, pixmap<double> &G )
        {
            assert(data.w==G.w);
            assert(data.h==G.h);
            const unit_t w = data.w;
            const unit_t h = data.h;
            
            double         Gmax = 0.0;
            for(unit_t j=1,jm=0,jp=2;jp<h;++j,++jm,++jp)
            {
                pixmap<double>::row           &Gj  = G[j];
                const typename pixmap<T>::row &dj  = data[j];
                const typename pixmap<T>::row &djm = data[jm];
                const typename pixmap<T>::row &djp = data[jp];
                
                for(unit_t i=1,im=0,ip=1;ip<w;++i,++im,++ip)
                {
                    const double gx = double(dj[ip])-double(dj[im]);
                    const double gy = double(djp[i])-double(djm[i]);
                    const double gg = math::Hypotenuse(gx, gy);
                    Gj[i] = gg;
                    if(gg>Gmax)
                    {
                        Gmax = gg;
                    }
                }
            }
            return Gmax;
        }
        
        template <typename T>
        void gradient( pixmap<T> &grad, const pixmap<T> &data ) throw()
        {
            
            assert(grad.w == data.w );
            assert(grad.h == data.h );
            const unit_t w = data.w;
            const unit_t h = data.h;
            
            // first pass: build 'real' gradient
            pixmap<double> G(w,h);
            const double Gmax = gradient_of(data, G);
            
            // second pass: convert to type
            if(Gmax<=0)
            {
                grad.ldz();
            }
            else
            {
                for(unit_t j=0;j<h;++j)
                {
                    typename pixmap<T>::row         &gj  = grad[j];
                    const pixmap<double>::row       &Gj  = G[j];
                    
                    for(unit_t i=0;i<w;++i)
                    {
                        gj[i] = to_unit<T>(Gj[i]/Gmax);
                    }
                }
            }
            
            
            
            
            
        }
        
    }
    
}

#endif
