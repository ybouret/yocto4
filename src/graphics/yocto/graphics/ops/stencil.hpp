#ifndef YOCTO_GRAPHICS_OPS_STENCIL_INCLUDED
#define YOCTO_GRAPHICS_OPS_STENCIL_INCLUDED 1

#include "yocto/graphics/pixmap.hpp"
#include "yocto/graphics/xpatch.hpp"


namespace yocto
{
    namespace graphics
    {

        class stencil
        {
        public:
            stencil() throw();
            ~stencil() throw();

            float     v[3][3];
            inline float * operator[](const unit_t dy) throw()
            {
                assert(dy);
                return v[dy+1];
            }
        };

    }
}

#endif
