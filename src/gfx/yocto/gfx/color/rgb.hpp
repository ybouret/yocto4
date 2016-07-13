#ifndef YOCTO_GFX_COLOR_RGB_INCLUDED
#define YOCTO_GFX_COLOR_RGB_INCLUDED 1

#include "yocto/graphics/pixel.hpp"

namespace yocto
{
    namespace gfx
    {

        class rgb
        {
        public:
            typedef T type;
            T r,g,b;
            inline rgb() throw() : r(0), g(0), b(0) {}
            inline rgb(const T R,const T G, const T B) throw() : r(R), g(G), b(B) {}
            inline rgb(const rgb &other) throw() : r(other.r),g(other.g),b(other.b) {}
            inline rgb & operator=( const rgb &other ) throw()
            {
                r = other.r; g=other.g; b=other.b;
                return *this;
            }
        };
    }
}

#endif
