#ifndef YOCTO_GRAPHICS_OPS_BLEND_INCLUDED
#define YOCTO_GRAPHICS_OPS_BLEND_INCLUDED 1

#include "yocto/graphics/rgb.hpp"
#include "yocto/graphics/pixmap.hpp"
#include "yocto/graphics/parallel.hpp"

namespace yocto
{
    namespace graphics
    {

        struct blend
        {
            static
            inline RGB mix( const RGB &lhs, const RGB &rhs, const RGB::type alpha ) throw()
            {
                const int        av = alpha;
                const int        au = 255-av;
                const RGB::type *u  = &lhs.r;
                const RGB::type *v  = &rhs.r;
                return RGB(
                           (( int(u[0])*au + int(v[0])*av )/255),
                           (( int(u[1])*au + int(v[1])*av )/255),
                           (( int(u[2])*au + int(v[2])*av )/255)
                           );
            }

            static inline float mix(const float lhs,const float rhs,const float alpha) throw()
            {
                return lhs*alpha + rhs*(1.0-alpha);
            }


            class patch : public graphics::patch
            {
            public:
                void       *target;
                const void *source;
                const void *args;

                virtual ~patch() throw();
                explicit patch(const graphics::patch &p) throw();
                patch(const patch &p) throw();

                template <typename T>
                void apply( lockable & ) throw()
                {
                    assert(source);
                    assert(target);
                    const pixmap<T> &src = *static_cast<const pixmap<T> *>(source);
                    pixmap<T>       &tgt = *static_cast<pixmap<T>       *>(target);

                    for(unit_t j=lower.y;j<=upper.y;++j)
                    {
                        const typename pixmap<T>::row       &sj = src[j];
                        typename pixmap<T>::row             &tj = tgt[j];
                        for(unit_t i=lower.x;i<=upper.x;++i)
                        {

                        }
                    }

                }

            private:
                YOCTO_DISABLE_ASSIGN(patch);
            };
            
            typedef vector<patch> patches;

            static void create(patches              &gp,
                               const graphics::patch &surface,
                               threading::engine    *server);


            template <typename T>
            static inline
            void apply(pixmap<T>         &tgt,
                       const pixmap<T>   &src,
                       patches           &bp,
                       threading::engine *server)
            {
                assert(tgt.w==src.w);
                assert(tgt.h==src.h);
                faked_lock sequential;
                for(size_t i=bp.size();i>0;--i)
                {
                    patch &sub = bp[i];
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


        };


    }
}

#endif
