#ifndef YOCTO_MATH_ALGO2D_HULL_INCLUDED
#define YOCTO_MATH_ALGO2D_HULL_INCLUDED 1

#include "yocto/geom/v2d.hpp"
#include "yocto/sequence/array.hpp"

namespace yocto
{
 
    namespace math
    {
        
        namespace algo2d
        {
            
            template <typename T>
            struct hull
            {
                typedef geom::v2d<T> vertex;
                void build( sequence<size_t> &h, const array<vertex> &points);
            };
            
        }
        
    }
    
}

#endif

