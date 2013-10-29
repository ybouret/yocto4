#ifndef YOCTO_GFX_RGB_INCLUDED
#define YOCTO_GFX_RGB_INCLUDED 1

#include "yocto/gfx/types.hpp"

namespace yocto
{
    namespace gfx
    {
        template <typename T>
        struct alpha_max;
        
        template <>
        struct alpha_max<uint8_t>
        {
            enum { value = 0xff };
        };
        
        
        template <>
        struct alpha_max<float>
        {
            enum { value = 1 };
        };
        
        //! generic rbg(a)
        template <typename T>
        class rgb
        {
        public:
            T r,g,b,a;
            
            inline rgb() throw() :
            r(0),g(0),b(0),a( alpha_max<T>::value ) {}
            
            inline rgb( T R, T G, T B, T A ) throw() :
            r(R), g(G), b(B), a(A) {}
            
            inline rgb( T R, T G, T B ) throw() :
            r(R), g(G), b(B), a( alpha_max<T>::value ) {}
            
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
        
        typedef rgb<uint8_t>  rgb_t;
        typedef rgb<uint32_t> rgb_mask_t;
        unsigned rgb_distance_sq( const rgb_t &lhs, const rgb_t &rhs ) throw();
    }
}

#endif
