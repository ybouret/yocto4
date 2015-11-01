#ifndef YOCTO_GRAPHIX_OPS_CHANNELS_INCLUDED
#define YOCTO_GRAPHIX_OPS_CHANNELS_INCLUDED 1

#include "yocto/graphix/pixmaps.hpp"
#include "yocto/graphix/parallel.hpp"

namespace yocto
{
    namespace graphix
    {

        struct channels
        {

            class patch  : public graphix:: patch
            {
            public:
                const void *source;
                void       *target;
                explicit patch(const graphix::patch &p) throw();
                virtual ~patch() throw();
                patch(const patch &other) throw();

                template <typename T,typename RGB_TYPE>
                void split(lockable &) throw()
                {
                    assert(source);
                    assert(target);
                    pixmaps<T>             &ch  = *static_cast<pixmaps<T> *>(target);
                    const pixmap<RGB_TYPE> &src = *static_cast<const pixmap<RGB_TYPE> *>(source);
                    const size_t nch = min_of<size_t>(ch.size,sizeof(RGB_TYPE)/sizeof(T));

                    assert(ch.w==src.w);
                    assert(ch.h==src.h);

                    for(unit_t j=lower.y;j<=upper.y;++j)
                    {
                        for(unit_t i=lower.x;i<=upper.x;++i)
                        {
                            const RGB_TYPE &p = src[j][i];
                            T              *q = (T *)&p;
                            for(size_t c=0;c<nch;++c)
                            {
                                ch[c][j][i] = q[c];
                            }
                        }
                    }

                }


            private:
                YOCTO_DISABLE_ASSIGN(patch);
            };

            typedef vector<patch> patches;
            static  void create(patches              &chp,
                                const graphix::patch &surface,
                                threading::engine    *server);


            template <typename T,typename RGB_TYPE>
            static inline void split(pixmaps<T>             &ch,
                                     const pixmap<RGB_TYPE> &src,
                                     patches                &chp,
                                     threading::engine      *server)
            {
                assert(ch.w==src.w);
                assert(ch.h==src.h);
                faked_lock access;
                for(size_t i=chp.size();i>0;--i)
                {
                    patch &sub = chp[i];
                    sub.target = &ch;
                    sub.source = &src;
                    if(server)
                    {
                        server->enqueue( &sub, & patch::split<T,RGB_TYPE> );
                    }
                    else
                    {
                        sub.split<T,RGB_TYPE>(access);
                    }
                }
                if(server) server->flush();
            }
            
            
        };
    }
}

#endif
