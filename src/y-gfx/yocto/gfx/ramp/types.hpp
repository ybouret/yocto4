#ifndef YOCTO_GFX_RAMP_TYPES_INCLUDED
#define YOCTO_GFX_RAMP_TYPES_INCLUDED 1

#include "yocto/gfx/rgb.hpp"

namespace yocto
{
    namespace gfx
    {
        
        struct ramp
        {
            static inline rgb_t get(float v) throw();
            template <typename T>
            static inline float rescale(T v, const T vmin, const T vmax) throw()
            {
                if(v<=vmin) return 0.0f;
                if(v>=vmax) return 1.0f;
                return (v-vmin)/(vmax-vmin);
            }
            
            template <typename T>
            static inline rgb_t to_rgb(T v,const T vmin=0,const T vmax=1) throw()
            {
                return get( rescale(v,vmin,vmax) );
            }
        };
        
    }
}

#endif
