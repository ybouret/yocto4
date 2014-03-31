#include "yocto/fovea/cell/line.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    
    namespace fovea
    {
        void check_line_vertices(const VertexBase &a, const VertexBase &b)
        {
            if(&a == &b)
                throw exception("Line with same Vertices!");
            if(a.index==b.index)
                throw exception("UNEXPECTED Line with same Vertices INDEX!!!");
        }
        
        
        void check_line_edge(const void *addr,const edge_key &ek)
        {
            if(!addr)
                throw exception("no edge for line from Vertex #%u -> Vertex #%u", unsigned(ek.i1), unsigned(ek.i2) );
        }
    }
}
