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
                   const size_t    nv,
                   const form_type f,
                   const size_t    s) throw() :
        dims(d),
        vertices(nv),
        form(f),
        real(sz2fp(s)),
        adb(a)
        {
            assert(1==dims||2==dims||3==dims);
            assert(vertices>0);
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
        
        const char * mesh::axis_name( size_t dim )
        {
            switch (dim)
            {
                case 0:
                    return "X";
                    
                case 1:
                    return "Y";
                    
                case 2:
                    return "Z";
                    
                default:
                    break;
            }
            throw exception("mesh: invalid dimension index");
        }
        
        
    }
    
}