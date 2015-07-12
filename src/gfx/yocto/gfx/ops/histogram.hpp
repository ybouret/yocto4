#ifndef YOCTO_GFX_OPS_HISTOGRAM_INCLUDED
#define YOCTO_GFX_OPS_HISTOGRAM_INCLUDED 1

#include "yocto/gfx/types.hpp"

namespace yocto
{
    namespace gfx
    {
        
        class histogram
        {
        public:
            static const size_t bins = 256;
            size_t              count[bins];
            
        private:
            
        };
        
    }
}

#endif
