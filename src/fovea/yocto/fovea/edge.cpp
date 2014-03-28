#include "yocto/fovea/edge.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    
    namespace fovea
    {
        void check_edge(const VertexBase &a, const VertexBase &b)
        {
            if(&a == &b)
                throw exception("Edge with same Vertices!");
            if(a.index==b.index)
                throw exception("UNEXPECTED Edge with same Vertices INDEX!!!");
        }
        
    }
}