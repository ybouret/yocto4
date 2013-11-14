#ifndef YOCTO_GFX_RAMP_TYPES_INCLUDED
#define YOCTO_GFX_RAMP_TYPES_INCLUDED 1

#include "yocto/gfx/rgb.hpp"

namespace yocto
{
    namespace gfx
    {
        
        struct ramp
        {
            static rgb_t get(float v) throw();
            static rgb_t get(float v, const rgb_t *colors) throw(); //!< assume 256 colors !
            
            template <typename T>
            static inline float rescale(T v, const T vmin, const T vmax) throw()
            {
                if(v<=vmin) return 0.0f;
                if(v>=vmax) return 1.0f;
                return float(v-vmin)/float(vmax-vmin);
            }
            
            template <typename T>
            static inline rgb_t to_rgb(T v,const T vmin,const T vmax) throw()
            {
                return get( rescale(v,vmin,vmax) );
            }
            
            template <typename T>
            static inline rgb_t to_rgb(T v, const rgb_t *colors, const T vmin,const T vmax) throw()
            {
                const float u = rescale(v,vmin,vmax);
                return colors ? get(u,colors) : get(u);
            }
        };
        
    }
}

#endif
