#ifndef YOCTO_GFX_RGB_INCLUDED
#define YOCTO_GFX_RGB_INCLUDED 1

#include "yocto/gfx/types.hpp"
#include "yocto/functor.hpp"

namespace yocto
{
    namespace gfx
    {
              
#define YOCTO_GFX_RGB_DECL(RGB,TYPE) \
TYPE r,g,b,a;\
RGB() throw(); \
RGB(const RGB &) throw();\
RGB &operator=(const RGB &) throw();\
RGB(TYPE R,TYPE G,TYPE B) throw();\
RGB(TYPE R,TYPE G,TYPE B,TYPE A) throw();\
friend bool operator==(const RGB &,const RGB &) throw();\
~RGB() throw()
        
#define YOCTO_GFX_RGB_IMPL(RGB,TYPE,TMAX) \
RGB:: RGB() throw() : r(0), g(0), b(0), a( TMAX) {} \
RGB::~RGB() throw() {}\
RGB::RGB(const RGB &other) throw() : r(other.r), g(other.g), b(other.b), a(other.a) {}\
RGB & RGB:: operator=(const RGB &other) throw() \
{ r=other.r; g=other.g; b=other.b; a=other.a; return *this; }\
RGB:: RGB( TYPE R, TYPE G, TYPE B ) throw() : r(R), g(G), b(B), a(TMAX) {}\
RGB:: RGB( TYPE R, TYPE G, TYPE B, TYPE A ) throw() : r(R), g(G), b(B), a(A) {}\
bool operator==( const RGB &lhs, const RGB &rhs ) throw() \
{ return (lhs.r == rhs.r) && (lhs.g == rhs.g) && (lhs.b == rhs.b) && (lhs.a == rhs.a); }
        
        class rgb_mask_t
        {
        public:
            YOCTO_GFX_RGB_DECL(rgb_mask_t,uint32_t);
        };
        
        class rgb_t
        {
        public:
            YOCTO_GFX_RGB_DECL(rgb_t, uint8_t);
            static unsigned distance_sq( const rgb_t &lhs, const rgb_t &rhs ) throw();
            uint8_t grey() const throw();
            rgb_t( const string &id);
            rgb_t( const char   *id);
        };
        
        
        typedef functor<rgb_t,TL1(const void*)> addr2rgba;
        
        
    }
}

#endif
