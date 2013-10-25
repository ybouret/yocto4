#ifndef YOCTO_GFX_REGION_INCLUDED
#define YOCTO_GFX_REGION_INCLUDED 1

#include "yocto/gfx/types.hpp"

namespace yocto
{
    namespace gfx
    {
        class region
        {
        public:
            const unit_t x,y;
            const size_t w,h;
            const unit_t xlast,ylast;
            
            region(unit_t X,unit_t Y,const size_t W,const size_t H) throw();
            ~region() throw();
            region(const region &other) throw();
            
        private:
            YOCTO_DISABLE_ASSIGN(region);
            
            
        };
    }
    
}

#endif

