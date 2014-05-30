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
            inline rgb( T R, T G, T B ) throw() : r(R), g(G), b(B) {}
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
        
        template <typename T>
        struct opaque
        {
            static const T value;
        };
        
        template <typename T>
        class rgba
        {
        public:
            T r,g,b,a;
            inline  rgba() throw() : r(0), g(0), b(0), a( opaque<T>::value ) {}
            inline  rgba( T R, T G, T B ) throw() : r(R), g(G), b(B), a( opaque<T>::value ) {}
            inline  rgba( T R, T G, T B, T A ) throw() : r(R), g(G), b(B), a(A) {}

            inline ~rgba() throw() {}
            inline rgba( const rgba &other ) throw() : r(other.r), g(other.g), b(other.b), a(other.a) {}
            inline rgba & operator=( const rgba &other ) throw()
            {
                r = other.r;
                g = other.g;
                b = other.b;
                a = other.a;
                return *this;
            }
            
        };
        
    }
}

YOCTO_SUPPORT_C_STYLE_OPS(gfx::rgb<uint8_t>);
YOCTO_SUPPORT_C_STYLE_OPS(gfx::rgb<uint32_t>);
YOCTO_SUPPORT_C_STYLE_OPS(gfx::rgb<float>);
YOCTO_SUPPORT_C_STYLE_OPS(gfx::rgb<double>);
YOCTO_SUPPORT_C_STYLE_OPS(gfx::rgba<uint8_t>);
YOCTO_SUPPORT_C_STYLE_OPS(gfx::rgba<uint32_t>);
YOCTO_SUPPORT_C_STYLE_OPS(gfx::rgba<float>);
YOCTO_SUPPORT_C_STYLE_OPS(gfx::rgba<double>);


#endif
