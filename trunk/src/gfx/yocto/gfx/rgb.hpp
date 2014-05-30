#ifndef YOCTO_GFX_RGB_INCLUDED
#define YOCTO_GFX_RGB_INCLUDED 1


#include "yocto/gfx/types.hpp"

namespace yocto
{
    namespace gfx
    {
        
        template <typename T>
        class rgb
        {
        public:
            T r,g,b;
            
            inline rgb() throw() : r(0), g(0), b(0) {}
            inline rgb( T R, T G, T B ) throw() : r(G), g(G), b(B) {}
            inline rgb( const rgb &other ) throw() : r(other.r), g(other.g), b(other.b) {}
            inline rgb & operator=( const rgb &other ) throw()
            {
                r = other.r;
                g = other.g;
                b = other.b;
                return *this;
            }
            
        };
        
        typedef rgb<uint8_t> rgb_t;
        
    }
}

#endif
