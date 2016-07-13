#ifndef YOCTO_GFX_COLOR_RGB_INCLUDED
#define YOCTO_GFX_COLOR_RGB_INCLUDED 1

#include "yocto/gfx/pixel.hpp"

namespace yocto
{
    namespace gfx
    {

        template <typename T>
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

            inline friend std::ostream & operator<<( std::ostream &os, const rgb<T> &C )
            {
                os << '(' << double(C.r) << ',' << double(C.g) << ',' << double(C.b) << ')';
                return os;
            }
        };

        template <typename T>
        class rgba
        {
        public:
            typedef T type;

            T r,g,b,a;
            inline rgba() throw() : r(0), g(0), b(0), a( pixel<T>::opaque ) {}
            inline rgba(const T R,const T G, const T B, const T A) throw() : r(R), g(G), b(B), a(A) {}
            inline rgba(const T R,const T G, const T B) throw() : r(R), g(G), b(B), a( pixel<T>::opaque ) {}
            inline rgba(const rgba &other) throw() : r(other.r),g(other.g),b(other.b), a(other.a) {}
            inline rgba & operator=( const rgba &other ) throw()
            {
                r = other.r; g=other.g; b=other.b; a=other.a;
                return *this;
            }

            inline rgba(const rgb<T> &other) throw() : r(other.r),g(other.g),b(other.b), a( pixel<T>::opaque ) {}

            inline rgb<T>       & _rgb() throw()       { return *(rgb<T>       *)this; }
            inline const rgb<T> & _rgb() const throw() { return *(const rgb<T> *)this; }

            inline friend std::ostream & operator<<( std::ostream &os, const rgba<T> &C )
            {
                os << '(' << double(C.r) << ',' << double(C.g) << ',' << double(C.b) << ',' << double(C.a) << ')';
                return os;
            }

        };

        typedef rgb<uint8_t>  RGB;
        typedef rgba<uint8_t> RGBA;
    }
}

#endif
