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
            const unit_t x; //!< x-offset
            const unit_t y; //!< y-offset
            const unit_t w; //!< width
            const unit_t h; //!< height
            const unit_t xend; //!< first invalid x-offset
            const unit_t yend; //!< first invalid y-offset
            
            region(unit_t X,unit_t Y,const unit_t W,const unit_t H) throw();
            ~region() throw();
            region(const region &other) throw();
            
        private:
            YOCTO_DISABLE_ASSIGN(region);
            
            
        };
    }
    
}

#endif

