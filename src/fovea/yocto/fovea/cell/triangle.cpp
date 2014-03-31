#include "yocto/fovea/cell/triangle.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace fovea
    {
        void check_triangle(const VertexBase &a, const VertexBase &b, const VertexBase &c)
        {
            if(&a==&b || &a==&c || &b==&c)
                throw exception("Triangle Invalid Vertices");
            if(a.index==b.index||a.index==c.index||b.index==c.index)
                throw exception("Unexpected Triangle Invalid Vertices INDEX");
        }

        void no_possible_triangle1D()
        {
            throw exception("No Possible Triangle1D");
        }
        
        
        void invalid_triangle2D()
        {
            throw exception("Invalid Triangle 2D");
        }
    }
}
