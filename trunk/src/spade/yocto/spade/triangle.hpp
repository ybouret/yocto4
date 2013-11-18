#ifndef YOCTO_SWAMP_TRIANGLE_INCLUDED
#define YOCTO_SWAMP_TRIANGLE_INCLUDED 1

#include "yocto/spade/in3d.hpp"

namespace yocto 
{
    namespace spade
    {
        template <typename U>
        class triangle3D
        {
        public:
            typedef typename vertex3D<U>::type vertex;
            
            vertex p0, p1, p2;
            triangle3D() throw();
            triangle3D( const triangle3D &f ) throw();
            triangle3D & operator=( const triangle3D &f ) throw();            
            ~triangle3D() throw();
            
            inline vertex       & operator[](size_t i) throw()       { assert(i<3); return *(&p0 + i); }
            inline const vertex & operator[](size_t i) const throw() { assert(i<3); return *(&p0 + i); }
        };
        
    }
}


#endif
