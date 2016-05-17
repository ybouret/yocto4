#ifndef YOCTO_GRAPHICS_OPS_GRADIENT_INCLUDED
#define YOCTO_GRAPHICS_OPS_GRADIENT_INCLUDED 1

#include "yocto/graphics/xpatch.hpp"
#include "yocto/graphics/pixmap.hpp"
#include "yocto/code/utils.hpp"

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

            inline explicit gradient() throw() {}
            inline virtual ~gradient() throw() {}

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
                    xp.enqueue(this, & gradient::apply<T>,server);
                }
            }
            
            template <typename T>
            inline void finish(pixmap<T>           &img,
                               const pixmap<float> &grd,
                               xpatches            &xps,
                               threading::engine   *server)
            {
                float gmax = 0;
                for(size_t i=xps.size();i>0;--i)
                {
                    gmax = max_of(gmax,xps[i].as<io_data>().gmax);
                }
                for(size_t i=xps.size();i>0;--i)
                {
                    xpatch  &xp   = xps[i];
                    io_data &data = xp.make<io_data>();
                    data.gmax     = gmax;
                    data.src      = &grd;
                    data.tgt      = &img;
                }
            }
            
            

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(gradient);
            template <typename T>
            void apply( xpatch &xp, lockable & ) throw()
            {
                io_data         &data  = xp.as<io_data>();
                pixmap<float>   &g     = *static_cast<pixmap<float>   *>(data.tgt);
                const pixmap<T> &s     = *static_cast<const pixmap<T> *>(data.src);
                const vertex     lower = xp.lower;
                const vertex     upper = xp.upper;
                float            gmax  = 0;
                for(unit_t j=upper.y;j>=lower.y;--j)
                {
                    pixmap<float>::row            &g_j = g[j];
                    const typename pixmap<T>::row &s_j = s[j];
                    for(unit_t i=upper.x;i>=lower.x;--i)
                    {
                        g_j[i] = s_j[i];
                        gmax   = max_of(gmax,g_j[j]);
                    }
                }
                data.gmax = gmax;
            }
            
            template <typename T>
            void update( xpatch &xp, lockable &) throw()
            {
                io_data               &data  = xp.as<io_data>();
                const pixmap<float>   &g     = *static_cast<const pixmap<float>   *>(data.src);
                pixmap<T>             &s     = *static_cast< pixmap<T>            *>(data.tgt);
                const vertex     lower = xp.lower;
                const vertex     upper = xp.upper;
                const float      gmax  = data.gmax;
                for(unit_t j=upper.y;j>=lower.y;--j)
                {
                    const pixmap<float>::row &g_j = g[j];
                    typename pixmap<T>::row  &s_j = s[j];
                    for(unit_t i=upper.x;i>=lower.x;--i)
                    {
                        s_j[i] = gist::float2byte( g_j[i]/gmax );
                    }
                    
                }
            }
        };
        
    }
    
}

#endif
