#include "yocto/fovea/vertex.hpp"

namespace yocto
{
    namespace fovea
    {
        VertexBase:: ~VertexBase() throw() {}
        VertexBase:: VertexBase(size_t idx) throw() : index(idx) {}
        VertexBase:: VertexBase(const VertexBase &other) throw() : index(other.index) {}
        
    }
}
