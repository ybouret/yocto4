#ifndef YOCTO_MATH_ALG_DELAUNAY_INCLUDED
#define YOCTO_MATH_ALG_DELAUNAY_INCLUDED 1

#include "yocto/geom/v3d.hpp"
#include "yocto/sequence/array.hpp"

namespace yocto
{
    
    namespace math
    {
        
        struct iTriangle
        {
            size_t p1, p2, p3;            
        };
        
        template <typename T>
        struct delaunay
        {
            typedef geom::v2d<T> vtx2d;
            typedef geom::v3d<T> vtx3d;
            
            static
            void build(const array<vtx2d> &);
            
            static
            void build(const array<vtx3d> &);
            
        };
        
    }
    
}

YOCTO_SUPPORT_C_STYLE_OPS(math::iTriangle);

#endif

