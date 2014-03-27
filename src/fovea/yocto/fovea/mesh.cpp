#include "yocto/fovea/mesh.hpp"

namespace yocto
{
    namespace fovea
    {
        
        mesh:: ~mesh() throw()
        {
        }
        
        mesh::mesh(const size_t    d,
                   const form_type f,
                   const real_type r) throw() :
        dimensions(d),
        form(f),
        real(r)
        {
        }
        
        
        
    }
    
}