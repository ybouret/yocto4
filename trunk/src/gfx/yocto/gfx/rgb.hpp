#ifndef YOCTO_GFX_RGB_INCLUDED
#define YOCTO_GFX_RGB_INCLUDED 1


#include "yocto/gfx/types.hpp"
#include <iostream>

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
            
            friend inline std::ostream & operator<<( std::ostream &os, const rgb<T> &C )
            {
                os << '[' << double(C.r) << ' ' << double(C.g) << ' ' << double(C.b) << ']';
                return os;
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
            inline rgba( const rgb<T> &other) throw() : r(other.r), g(other.g), b(other.b), a(opaque<T>::value) {}
            
            inline rgba & operator=( const rgba &other ) throw()
            {
                r = other.r;
                g = other.g;
                b = other.b;
                a = other.a;
                return *this;
            }
            
            //! Keep the alpha channel !
            inline rgba & operator=( const rgb<T> &other ) throw()
            {
                r = other.r;
                g = other.g;
                b = other.b;
                return *this;
            }

            
            
            friend inline std::ostream & operator<<( std::ostream &os, const rgba<T> &C )
            {
                os << '[' << double(C.r) << ' ' << double(C.g) << ' ' << double(C.b) << ' ' << double(C.a) << ']';
                return os;
            }
        };
        
        typedef rgba<uint8_t> rgba_t;

        template <typename RGB_TYPE>
        inline float rgb2gs(const RGB_TYPE &C) throw()
        {
            return conv::greyscale_f(C.r, C.g, C.b);
        }
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
