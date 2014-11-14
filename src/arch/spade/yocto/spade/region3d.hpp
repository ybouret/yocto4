#ifndef YOCTO_SPADE_REGION3D_INCLUDED
#define YOCTO_SPADE_REGION3D_INCLUDED 1

#include "yocto/spade/region.hpp"
#include "yocto/spade/in3d.hpp"

namespace yocto
{
    namespace spade
    {
        template <typename T> struct region3D
        {
            typedef region<T,vertex3D>           type;
        };
        
    }
}

#endif
