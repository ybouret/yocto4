#ifndef YOCTO_GFX_TYPES_INCLUDED
#define YOCTO_GFX_TYPES_INCLUDED 1

#include "yocto/string.hpp"


namespace yocto
{
    namespace gfx
    {
        
        template <typename T>
        class quad
        {
        public:
            inline quad() throw() : r(0), g(0), b(0), a(0) {}
            
            inline quad( const T R, const T G, const T B, const T A) throw() :
            r(R),
            g(G),
            b(B),
            a(A)
            {
            }
            
            inline ~quad() throw() {}
            
            T r,g,b,a;
    
            inline  quad( const quad &other ) throw() :
            r(other.r),
            g(other.g),
            b(other.b),
            a(other.a)
            {
            }
            
            inline quad & operator=( const quad &other ) throw()
            {
                r = other.r;
                g = other.g;
                b = other.b;
                a = other.a;
                return *this;
            }
        };
        
        typedef quad<uint32_t> quad32_t;
        typedef quad<uint8_t>  quad8_t;
        
        struct color_bits
        {
            //! find how many bits (0..8) and what shift, throw on error
            static void   fmt( const uint32_t mask, const uint8_t &bits, const uint8_t &shift, const uint8_t &loss, const char *which);
            static string binary(const uint32_t c);
        };
        
       
        
    }
}

#endif
