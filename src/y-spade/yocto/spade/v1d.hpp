#ifndef YOCTO_SPADE_V1D_INCLUDED
#define YOCTO_SPADE_V1D_INCLUDED 1


#include "yocto/os.hpp"

namespace yocto
{
    namespace geom
    {
        
        template <typename T>
        struct  v1d
        {
            T x;
            
            inline v1d() throw() : x(0) {}
            inline v1d( T a ) throw() : x(a) {}
            
            inline v1d( const v1d &v ) throw() : x(v.x) {}
            inline v1d& operator=(const v1d &v ) throw() { x=v.x; return *this; }
            
            inline v1d & operator+=(const v1d &v ) throw() { x += v.x; return *this; }
            inline v1d   operator+() const throw() { return *this; }
            inline v1d   operator+(const v1d &v) const throw() { return v1d(x+v.x); }
            
            
        };
        
    }
    
}

#endif
