#ifndef YOCTO_GEMS_V4D_INCLUDED
#define YOCTO_GEMS_V4D_INCLUDED 1

#include "yocto/math/types.hpp"


namespace yocto
{
    
    namespace gems
    {
        template <typename T>
        class v4d
        {
        public:
            T x,y,z,w;
            inline  v4d() throw() : x(0), y(0), z(0), w(0) {}
            inline  v4d(T X,T Y,T Z,T W=0) throw() : x(X), y(Y), z(Z), w(W) {}
            inline ~v4d() throw() {}
            
            inline v4d( const v4d &other ) throw() :
            x(other.x),y(other.y),z(other.z),w(other.w) {}
            
            inline v4d( const v4d &a, const v4d &b ) throw() :
            x(b.x-a.x), y(b.y-a.x), z(b.z-a.z), w(0) {}
            
            inline v4d & operator=( const v4d &other ) throw()
            {
                x = other.x;
                y = other.y;
                z = other.z;
                w = other.w;
                return *this;
            }
            
            inline void ldz3() throw()
            { x=y=z=0; }
            
            inline void set3(const v4d &other) throw()
            { x=other.x;y=other.y;z=other.z;}
            
            inline void add3(const v4d &other) throw()
            { x+=other.x;y+=other.y;z+=other.z; }
            
            inline void sub3(const v4d &other) throw()
            { x-=other.x;y-=other.y;z-=other.z; }
            
            inline void muladd3(const T a, const v4d &other) throw()
            { x += a*other.x; y += a*other.y; z += a*other.z; }
            
            inline T squared_norm3(void) const throw()
            { return (x*x) + (y*y) + (z*z); }
            
        };
    }
}


#endif
