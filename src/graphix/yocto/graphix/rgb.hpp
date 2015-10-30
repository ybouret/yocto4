#ifndef YOCTO_GRAPHIX_RGB_INCLUDED
#define YOCTO_GRAPHIX_RGB_INCLUDED 1

#include "yocto/graphix/types.hpp"

namespace yocto
{
    namespace graphix
    {

        template <typename T>
        struct channel_info
        {
            static const T opaque;
            static bool    is_zero(const T) throw();
        };

        template <typename T>
        class rgb
        {
        public:
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



        template <typename T>
        class rgba
        {
        public:
            T r,g,b,a;
            inline rgba() throw() : r(0), g(0), b(0), a( channel_info<T>::opaque ) {}
            inline rgba(const T R,const T G, const T B, const T A) throw() : r(R), g(G), b(B), a(A) {}
            inline rgba(const T R,const T G, const T B) throw() : r(R), g(G), b(B), a( channel_info<T>::opaque ) {}
            inline rgba(const rgba &other) throw() : r(other.r),g(other.g),b(other.b), a(other.a) {}
            inline rgba & operator=( const rgba &other ) throw()
            {
                r = other.r; g=other.g; b=other.b; a=other.a;
                return *this;
            }

            inline rgba(const rgb<T> &other) throw() : r(other.r),g(other.g),b(other.b), a( channel_info<T>::opaque ) {}

        };


        typedef rgb<uint8_t>  RGB;
        typedef rgba<uint8_t> RGBA;

        //______________________________________________________________________
        //
        // abstract operations
        //______________________________________________________________________
        template <typename T> bool    is_zero_pixel(const T&) throw();
        template <typename T> uint8_t project(const T&) throw();
        
    }
    
}

YOCTO_SUPPORT_C_STYLE_OPS(graphix::rgb<uint8_t>);
YOCTO_SUPPORT_C_STYLE_OPS(graphix::rgb<float>);
YOCTO_SUPPORT_C_STYLE_OPS(graphix::rgba<uint8_t>);
YOCTO_SUPPORT_C_STYLE_OPS(graphix::rgba<float>);



#endif
