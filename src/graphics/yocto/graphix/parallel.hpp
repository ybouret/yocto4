#ifndef YOCTO_GRAPHIX_PARALLEL_INCLUDED
#define YOCTO_GRAPHIX_PARALLEL_INCLUDED 1

#include "yocto/threading/engine.hpp"
#include "yocto/graphix/types.hpp"
#include "yocto/parallel/splitter.hpp"

namespace yocto
{
    namespace graphix
    {
        
        template <typename USER_PATCH>
        inline
        void prepare_patches(vector<USER_PATCH> &patches,
                             const size_t        cpus,
                             const patch        &src,
                             const bool          full)
        {
            unit_t x = src.lower.x;
            unit_t y = src.lower.y;
            unit_t w = src.width.x;
            unit_t h = src.width.y;
            if(!full)
            {
                if(w<=2||h<=2) throw exception("prepare_patches: source is too small");
                w -= 2;
                h -= 2;
                ++x;
                ++y;
            }
            --w;
            --h;
            const patch origin( vertex(x,y), vertex(x+w,y+h) );
            parallel::build_patches(patches,cpus,origin);
        }
        
    }
}

#endif
