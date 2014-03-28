#include "yocto/fovea/cell/line.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    
    namespace fovea
    {
        void check_line(const VertexBase &a, const VertexBase &b)
        {
            if(&a == &b)
                throw exception("Line with same Vertices!");
            if(a.index==b.index)
                throw exception("UNEXPECTED Line with same Vertices INDEX!!!");
        }
        
    }
}
