#ifndef YOCTO_GFX_RECTANGLE_INCLUDED
#define YOCTO_GFX_RECTANGLE_INCLUDED 1

#include "yocto/gfx/types.hpp"
#include "yocto/bitwise.hpp"

namespace yocto
{
    namespace gfx
    {
        
        class rectangle
        {
        public:
            rectangle(unit_t xorg, unit_t yorg, size_t width, size_t height) throw();
            rectangle(const rectangle &other) throw();
            ~rectangle() throw();
            
            const unit_t x;
            const unit_t y;
            const size_t w;
            const size_t h;
            const unit_t xout; //!< first invalid coordinate
            const unit_t yout; //!< first invalid coordinate
            
        private:
            YOCTO_DISABLE_ASSIGN(rectangle);
        };
        
    }
}

YOCTO_SUPPORT_C_STYLE_OPS(gfx::rectangle);

#endif
