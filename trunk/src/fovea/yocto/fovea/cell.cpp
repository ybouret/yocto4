#include "yocto/fovea/cell.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace fovea
    {
        size_t check_num_vertices( size_t nv )
        {
            if(nv<2)
                throw exception("Invalid Cell #Vertices");
            return nv;
        }

    }
}
