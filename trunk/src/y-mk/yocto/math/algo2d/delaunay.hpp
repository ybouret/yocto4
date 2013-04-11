#ifndef YOCTO_MATH_ALGO2D_DELAUNAY_INCLUDED
#define YOCTO_MATH_ALGO2D_DELAUNAY_INCLUDED 1

#include "yocto/geom/v2d.hpp"
#include "yocto/math/itriangle.hpp"

namespace yocto
{
    
    namespace math
    {
        
        namespace algo2d
        {
            
            template <typename T>
            struct delaunay
            {
                
                               
                typedef geom::v2d<T> vertex;
                static
                void build(iTriangle::List &trlist, const array<vertex> &vertices );
                
            };
        }
        
    }
    
}


#endif

