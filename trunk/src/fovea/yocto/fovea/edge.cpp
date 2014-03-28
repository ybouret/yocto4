#include "yocto/fovea/edge.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    
    namespace fovea
    {
        void check_edge(size_t i,size_t j)
        {
            if(i==j)
                throw exception("Edge with same Vertices!");
        }
        
    }
}