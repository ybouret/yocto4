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
            
            inline v4d & operator=( const v4d &other ) throw()
            {
                x = other.x;
                y = other.y;
                z = other.z;
                w = other.w;
                return *this;
            }
            
        };
    }
}


#endif
