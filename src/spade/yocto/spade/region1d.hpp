#ifndef YOCTO_SPADE_REGION1D_INCLUDED
#define YOCTO_SPADE_REGION1D_INCLUDED 1

#include "yocto/spade/region.hpp"
#include "yocto/spade/in1d.hpp"

namespace yocto
{
    namespace spade
    {
        template <typename T> struct region1D
        {
            typedef region<T,vertex1D>    type;
        };
        
    }
}

#endif
