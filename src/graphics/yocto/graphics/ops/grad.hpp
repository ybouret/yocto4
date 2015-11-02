#ifndef YOCTO_GRAPHIX_GRADIENT_INCLUDED
#define YOCTO_GRAPHIX_GRADIENT_INCLUDED 1

#include "yocto/graphics/rawpix.hpp"
#include "yocto/graphics/parallel.hpp"
#include <cmath>

namespace yocto
{
    namespace graphix
    {

        struct gradient
        {
            typedef float real_type;

            class patch : public graphix::patch
            {
            public:
                real_type   gmax;
                void       *target;
                const void *source;
                virtual ~patch() throw();
                explicit patch(const graphix::patch &p) throw();
                patch(const patch &p) throw();

                template <typename T>
                inline void eval(lockable &) throw()
                {
                    static const real_type four(4);
                    static const real_type three(3);

                    assert(source);
                    assert(target);

                    gmax = 0;

                    pixmap<real_type> &g = *static_cast< pixmap<real_type> *>(target);
                    const pixmap<T>   &p = *static_cast<const pixmap<T>    *>(source);
                    assert(g.w==p.w);
                    assert(g.h==p.h);
                    const unit_t h0 = g.h-1;
                    const unit_t h1 = h0-1;
                    const unit_t h2 = h0-2;
                    const unit_t w0 = g.w-1;
                    const unit_t w1 = w0-1;
                    const unit_t w2 = w0-2;

                    for(unit_t j=lower.y;j<=upper.y;++j)
                    {
                        const bool at_dn = (j<=0);
                        const bool at_up = (j>=h0);
                        pixmap<real_type>::row        &gj = g[j];
                        const typename pixmap<T>::row &pj = p[j];
                        for(unit_t i=lower.x;i<=upper.x;++i)
                        {
                            const real_type   pji = pj[i];
                            real_type         gx  = 0;
                            if(i<=0)
                            {
                                gx = four*pj[i+1] - three*pji + real_type(pj[i+2]);
                            }
                            else
                            {
                                if(i>=w0)
                                {
                                    gx = four*pj[w1] - three*pji + real_type(pj[w2]);
                                }
                                else
                                {
                                    gx = real_type(pj[i+1]) - real_type(pj[i-1]);
                                }
                            }

                            real_type gy = 0;
                            if(at_dn)
                            {
                                gy = four * p[j+1][i] - three*pji + real_type(p[j+2][i]);
                            }
                            else
                            {
                                if(at_up)
                                {
                                    gy = four * p[h1][i] - three*pji + real_type(p[h2][i]);
                                }
                                else
                                {
                                    gy = real_type(p[j+1][i]) - real_type(p[j-1][i]);
                                }
                            }



                            const real_type g = std::sqrt(gx*gx+gy*gy);
                            gj[i] = g;
                            if(g>gmax) gmax = g;
                        }


                    }

                }


                template <typename T>
                inline void apply(lockable &) throw()
                {
                    static const real_type amplitude( channel_info<T>::opaque );
                    assert(gmax>0);
                    assert(source);
                    assert(target);

                    const pixmap<real_type> &g = *static_cast<const pixmap<real_type> *>(source);
                    pixmap<T>               &p = *static_cast< pixmap<T> *>(target);
                    assert(g.w==p.w);
                    assert(g.h==p.h);

                    for(unit_t j=lower.y;j<=upper.y;++j)
                    {
                        const pixmap<real_type>::row        &gj = g[j];
                        typename pixmap<T>::row             &pj = p[j];
                        for(unit_t i=lower.x;i<=upper.x;++i)
                        {
                            const real_type weight = gj[i]/gmax;
                            pj[i] = T( amplitude * weight);
                        }
                    }
                }


            private:
                YOCTO_DISABLE_ASSIGN(patch);
            };

            typedef vector<patch> patches;

            static void create(patches              &gp,
                               const graphix::patch &surface,
                               threading::engine    *server);

            template <typename T>
            static inline void launch(pixmap<real_type>  &g,
                                      const pixmap<T>    &pxm,
                                      patches            &gp,
                                      threading::engine  *server)
            {
                assert(g.w==pxm.w);
                assert(g.h==pxm.h);
                if(pxm.w<3||pxm.h<3)
                {
                    throw exception("gradient: image is too small");
                }
                faked_lock access;
                for(size_t i=gp.size();i>0;--i)
                {
                    patch &sub = gp[i];
                    sub.source = &pxm;
                    sub.target = &g;
                    if(server)
                    {
                        server->enqueue( &sub, & patch::eval<T> );
                    }
                    else
                    {
                        sub.eval<T>(access);
                    }
                }
            }

            template <typename T>
            static inline double finish(pixmap<T>               &grad,
                                        const pixmap<real_type> &g,
                                        patches                 &gp,
                                        threading::engine       *server)
            {
                if(server) server->flush();
                real_type  gmax = 0;
                for(size_t i=gp.size();i>0;--i)
                {
                    const real_type tmp = gp[i].gmax;
                    if(tmp>gmax) gmax=tmp;
                }
                std::cerr << "gmax=" << gmax << std::endl;
                if(gmax<=0)
                {
                    grad.ldz();
                }
                else
                {
                    faked_lock access;
                    for(size_t i=gp.size();i>0;--i)
                    {
                        patch &sub = gp[i];
                        sub.gmax   = gmax;
                        sub.source = &g;
                        sub.target = &grad;
                        if(server)
                        {
                            server->enqueue( &sub, & patch::apply<T> );
                        }
                        else
                        {
                            sub.apply<T>(access);
                        }
                    }
                    if(server) server->flush();
                }
                return gmax;
            }
            
            
            
            
        };
        
    }
}

#endif

