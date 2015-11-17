#ifndef YOCTO_GRAPHICS_BLUR_INCLUDED
#define YOCTO_GRAPHICS_BLUR_INCLUDED 1

#include "yocto/graphics/pixmaps.hpp"
#include "yocto/graphics/parallel.hpp"
#include <cmath>
#include "yocto/container/matrix.hpp"
#include "yocto/code/utils.hpp"

namespace yocto {

    namespace graphics
    {

        class blur : public pixmap<float>
        {
        public:
            class patch : public graphics::patch
            {
            public:
                void       *handle;
                void       *target;
                const void *source;
                explicit patch(const graphics::patch &p) throw();
                virtual ~patch() throw();
                patch(const patch &) throw();

                void compute( lockable & ) throw();

                template <typename T>
                inline void apply( lockable & ) throw()
                {
                    assert(handle);
                    assert(source);
                    assert(target);
                    assert(source!=target);
                    const blur      &blr = *static_cast<blur            *>(handle);
                    pixmap<T>       &tgt = *static_cast<pixmap<T>       *>(target);
                    const pixmap<T> &src = *static_cast<const pixmap<T> *>(source);

                    for(unit_t j=lower.y;j<=upper.y;++j)
                    {
                        for(unit_t i=lower.x;i<=upper.x;++i)
                        {
                            const float ans = blr.get_blurred(src,i,j);
                            tgt[j][i] = ans;
                        }
                    }

                }



            private:
                YOCTO_DISABLE_ASSIGN(patch);
            };

            explicit blur( const graphics::patch &surface, threading::engine *server );
            virtual ~blur() throw();

            void compute(const float sigma, threading::engine *server);

            template <typename T>
            inline
            void apply(pixmap<T>         &tgt,
                       const pixmap<T>   &src,
                       threading::engine *server)
            {
                assert(tgt.w==src.w);
                assert(tgt.h==src.h);
                faked_lock sequential;
                for(size_t i=patches.size();i>0;--i)
                {
                    patch &sub = patches[i];
                    sub.handle = this;
                    sub.source = &src;
                    sub.target = &tgt;
                    if(server)
                    {
                        server->enqueue(&sub, &patch::apply<T>);
                    }
                    else
                    {
                        sub.apply<T>(sequential);
                    }
                }
                if(server) server->flush();
            }

            template <typename T>
            inline float get_blurred(const pixmap<T> &src, unit_t i, unit_t j) const throw()
            {

                const unit_t         jmin = max_of<unit_t>(0,j-delta);
                const unit_t         jmax = min_of<unit_t>(upper.y,j+delta);

                const unit_t         imin = max_of<unit_t>(0,i-delta);
                const unit_t         imax = min_of<unit_t>(upper.x,i+delta);
                const pixmap<float> &self = *this;
                float num = 0;
                float den = 0;
                for(unit_t v=jmin;v<=jmax;++v)
                {
                    unit_t dv = v-j;
                    if(dv<0) { dv=-dv; }
                    for(unit_t u=imin;u<=imax;++u)
                    {
                        unit_t du = u-i;
                        if(du<0) { du=-du; }
                        const float weight = self[dv][du];
                        num += weight * float(src[v][u]);
                        den += weight;
                    }
                }
                return num/den;
            }


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(blur);
            vector<patch> patches;
            float         scaling;
            unit_t        delta;
        };
        
        
    }
}

#endif

