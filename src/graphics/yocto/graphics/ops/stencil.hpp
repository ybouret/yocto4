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
            stencil(const stencil &) throw();
            stencil & operator=(const stencil &) throw();

            float     v[3][3];
            inline float * operator[](const unit_t dy) throw()
            {
                assert(dy>=-1);assert(dy<=1);
                return &v[dy+1][1];
            }

            friend  std::ostream & operator<<( std::ostream &os, const stencil &S );

            template <typename T>
            void load(const pixmap<T> &pxm,
                      const vertex    &v)
            {
                assert(pxm.has(v));
            }

        };

    }
}

#endif
