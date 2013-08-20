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
            T r,g,b,a;
            
            inline rgb() throw() :
            r(0),g(0),b(0),a( alpha_max<T>::value ) {}
            
            inline rgb( const rgb &other ) throw() :
            r(other.r),
            g(other.g),
            b(other.b),
            a(other.a)
            {
            }
            
            inline rgb & operator=( const rgb &other ) throw()
            {
                r = other.r;
                g = other.g;
                b = other.b;
                a = other.a;
                return *this;
            }
            
            
            inline ~rgb() throw() {}
            
        };
        
        typedef rgb<uint8_t>  rgba_t;
        typedef rgb<uint32_t> rgba_mask_t;
        
    }
}

#endif
