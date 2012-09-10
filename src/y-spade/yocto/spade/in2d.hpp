#ifndef YOCTO_SPADE_IN2D_INCLUDED
#define YOCTO_SPADE_IN2D_INCLUDED 1

#include "yocto/spade/layout.hpp"
#include "yocto/geom/v2d.hpp"

namespace yocto
{
    
    namespace spade
    {
        typedef geom::v2d<ptrdiff_t>     coord2D;
        typedef layout_of<coord2D>       layout2D;
        template <typename T> struct     vertex2D {
            typedef geom::v2d<T> type;
        };
    }
    
}

#endif
