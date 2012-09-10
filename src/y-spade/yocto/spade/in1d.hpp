#ifndef YOCTO_SPADE_IN1D_INCLUDED
#define YOCTO_SPADE_IN1D_INCLUDED 1

#include "yocto/spade/layout.hpp"

namespace yocto
{
    
    namespace spade
    {
        typedef ptrdiff_t            coord1D;
        typedef layout_of<coord1D>   layout1D;
        template <typename T> struct vertex1D {
            typedef T type;
        };
    }
    
}

#endif
