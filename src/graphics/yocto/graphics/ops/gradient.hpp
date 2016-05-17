#ifndef YOCTO_GRAPHICS_OPS_GRADIENT_INCLUDED
#define YOCTO_GRAPHICS_OPS_GRADIENT_INCLUDED 1

#include "yocto/graphics/xpatch.hpp"
#include "yocto/graphics/pixmap.hpp"

namespace yocto
{
    namespace graphics
    {

        class gradient
        {
        public:

            struct io_data
            {
                float       gmax;
                void       *tgt;
                const void *src;
            };

            explicit gradient() throw() {}
            virtual ~gradient() throw() {}

            template <typename T>
            inline void start(pixmap<float>      &grd,
                              const pixmap<T>    &img,
                              xpatches           &xps,
                              threading::engine *server)
            {
                for(size_t i=xps.size();i>0;--i)
                {
                    xpatch  &xp   = xps[i];
                    io_data &data = xp.make<io_data>();
                    data.gmax     = 0;
                    data.tgt      = &grd;
                    data.src      = &img;
                    if(server)
                    {
                        xp.enqueue(this, & gradient::apply<T>, *server);
                    }
                    else
                    {
                        xp.execute(this, &gradient::apply<T>);
                    }
                }
            }


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(gradient);
            template <typename T>
            void apply( xpatch &xp, lockable & ) throw()
            {
                io_data         &data = xp.as<io_data>();
                pixmap<float>   &g    = *static_cast<pixmap<float>   *>(data.tgt);
                const pixmap<T> &s    = *static_cast<const pixmap<T> *>(data.src);
                const vertex     lower = xp.lower;
                const vertex     upper = xp.upper;
                for(unit_t j=upper.y;j>=lower.y;--j)
                {
                    pixmap<float>::row            &g_j = g[j];
                    const typename pixmap<T>::row &s_j = s[j];
                    for(unit_t i=upper.x;i>=lower.x;--i)
                    {
                        g_j[i] = 0.0f;
                    }
                }
            }
        };
        
    }
    
}

#endif
