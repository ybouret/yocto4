#include "yocto/fovea/cell/quad.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace fovea
    {
        void check_quad(const VertexBase &a, const VertexBase &b, const VertexBase &c, const VertexBase &d)
        {
            if(&a==&b || &a==&c || &a==&d ||
               &b==&c || &b==&d ||
               &c==&d)
                throw exception("Quad Invalid Vertices");
            if(a.index==b.index||a.index==c.index||a.index==d.index||
               b.index==c.index||b.index==d.index||
               c.index==d.index)
                throw exception("Unexpected Quad Invalid Vertices INDEX");
        }
        
        void no_possible_quad1D()
        {
            throw exception("No Possible 1D Quad");
        }
        
        void invalid_quad2D()
        {
            
        }
        
    }
}
