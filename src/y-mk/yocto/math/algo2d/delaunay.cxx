#include "yocto/math/algo2d/delaunay.hpp"
#include "yocto/math/ztype.hpp"

namespace yocto
{
    namespace math
    {
     
        namespace algo2d
        {
            template <>
            void delaunay<real_t>:: build(iTriangle::List &trlist, sequence<vertex> &vertices)
            {
            }
            
        }
    }
    
}
