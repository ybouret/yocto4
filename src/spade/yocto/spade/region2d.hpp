#ifndef YOCTO_SPADE_REGION2D_INCLUDED
#define YOCTO_SPADE_REGION2D_INCLUDED 1

#include "yocto/spade/region.hpp"
#include "yocto/spade/in2d.hpp"

namespace yocto
{
    namespace spade
    {
        template <typename T> struct region2D
        {
            typedef region<T,vertex2D>           type;
        };
        
    }
}

#endif
