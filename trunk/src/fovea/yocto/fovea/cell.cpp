#include "yocto/fovea/cell.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace fovea
    {
        static inline
        size_t check_num_vertices( size_t nv )
        {
            if(nv<2)
                throw exception("Invalid Cell #Vertices");
            return nv;
        }
        
        ICell:: ~ICell() throw()
        {
        }
        
        ICell:: ICell( size_t nv ) :
        vertices( check_num_vertices(nv) )
        {
            
        }
        

    }
}
