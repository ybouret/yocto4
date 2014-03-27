#include "yocto/fovea/mesh.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace fovea
    {
        
        mesh:: ~mesh() throw()
        {
        }
        
        mesh::mesh(array_db       &a,
                   const size_t    d,
                   const form_type f,
                   const real_type r) throw() :
        dimensions(d),
        form(f),
        real(r),
        adb(a)
        {
        }
        
        mesh::real_type mesh::sz2fp( const unsigned sz )
        {
            switch(sz)
            {
                case 4:
                    return use_float;
                    
                case 8:
                    return use_double;
                    
                default:
                    break;
            }
            throw exception("mesh: invalid floating point precision");
        }

        
        
    }
    
}