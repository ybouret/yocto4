#ifndef YOCTO_GRAPHICS_BLUR_INCLUDED
#define YOCTO_GRAPHICS_BLUR_INCLUDED 1

#include "yocto/graphics/pixmaps.hpp"
#include "yocto/graphics/parallel.hpp"
#include <cmath>
namespace yocto {
    
    namespace graphics
    {
    
        struct blur
        {
            typedef float  real_type;
            static  size_t level;
            
            class patch : public graphics::patch
            {
            public:
                void              *handle;
                pixmap<real_type> *blurred;
                real_type          sigma;
                
                explicit patch(const graphics::patch &p) throw();
                virtual ~patch() throw();
                patch(const patch &) throw();
                
                template <typename T>
                inline void compute(lockable &) throw()
                {
                    assert(handle);
                    assert(blurred);
                    assert(sigma>0);
                    
                    pixmap<real_type> &tgt     = *static_cast<pixmap<real_type>*>(blurred);
                    const pixmap<T>   &src     = *static_cast<const pixmap<T> *>(handle);
                    
                    for(unit_t j=lower.y;j<=upper.y;++j)
                    {
                        for(unit_t i=lower.x;i<=upper.x;++i)
                        {
                            tgt[j][i] = blur_of(src, sigma, i, j);
                        }
                    }
                    
                }
                
                template <typename T>
                inline void transfer(lockable &) throw()
                {
                    assert(handle);
                    assert(blurred);
                    const pixmap<real_type> &src = *static_cast<const pixmap<real_type>*>(blurred);
                    pixmap<T>               &tgt = *static_cast<pixmap<T> *>(handle);
                    
                    for(unit_t j=lower.y;j<=upper.y;++j)
                    {
                        for(unit_t i=lower.x;i<=upper.x;++i)
                        {
                            tgt[j][i] = src[j][i];
                        }
                    }

                }
                
            private:
                YOCTO_DISABLE_ASSIGN(patch);
                template <typename T> static inline
                real_type blur_of( const pixmap<T> &pxm,  real_type sigma, unit_t i, unit_t j) throw()
                {
                    assert(i>=0); assert(i<pxm.w);
                    assert(j>=0); assert(j<pxm.h);
                    
                    const real_type sig2     = sigma*sigma;
                    const real_type twoSig2  = sig2+sig2;
                    const real_type delta_sq = sig2 * std::log( real_type(2+blur::level) );
                    const real_type delta_r  = std::ceil(std::sqrt(delta_sq));
                    const unit_t    delta    = unit_t(delta_r);
                    real_type num  = 0;
                    real_type den  = 0;
                    
                    const unit_t vlo = max_of<unit_t>(j-delta,0);
                    const unit_t vhi = min_of<unit_t>(j+delta,pxm.upper.y);
                    
                    const unit_t ulo = max_of<unit_t>(i-delta,0);
                    const unit_t uhi = min_of<unit_t>(i+delta,pxm.upper.x);
                    
                    for(unit_t v=vlo;v<=vhi;++v)
                    {
                        const real_type dv(v-j);
                        const real_type dv2=dv*dv;
                        for(unit_t u=ulo;u<=uhi;++u)
                        {
                            const real_type du(u-i);
                            const real_type du2=du*du;
                            const real_type g = std::exp(-(du2+dv2)/twoSig2);
                            num += g*real_type(pxm[v][u]);
                            den += g;
                        }
                    }
                    return num/den;
                }

            };
            
            typedef vector<patch> patches;
            
            static void create(patches               &blr,
                               const graphics::patch &surface,
                               threading::engine     *server);
            
            
            template <typename T> static inline
            void launch(patches           &blr,
                        pixmap<real_type> &tgt,
                        pixmap<T>         &src,
                        const real_type    sigma,
                        threading::engine *server)
            {
                assert(tgt.w==src.w);
                assert(tgt.h==src.h);
                faked_lock sequential;
                for(size_t i=blr.size();i>0;--i)
                {
                    patch &sub  = blr[i];
                    sub.handle  = &src;
                    sub.blurred = &tgt;
                    sub.sigma   = sigma;
                    if(server)
                    {
                        server->enqueue(&sub, &patch::compute<T>);
                    }
                    else
                    {
                        sub.compute<T>(sequential);
                    }
                }
            }
            
            template <typename T> static inline
            void finish(patches           &blr,
                        pixmap<real_type> &field,
                        pixmap<T>         &pxm,
                        threading::engine *server)
            {
                assert(field.w==pxm.w);
                assert(field.h==pxm.h);
                if(server) server->flush();
                faked_lock sequential;
                for(size_t i=blr.size();i>0;--i)
                {
                    patch &sub  = blr[i];
                    sub.handle  = &pxm;
                    sub.blurred = &field;
                    if(server)
                    {
                        server->enqueue(&sub, &patch::transfer<T>);
                    }
                    else
                    {
                        sub.transfer<T>(sequential);
                    }
                }
                server->flush();
            }
            
            
        };
        
    }
}

#endif

