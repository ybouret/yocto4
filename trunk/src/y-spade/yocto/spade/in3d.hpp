#ifndef YOCTO_SPADE_IN3D_INCLUDED
#define YOCTO_SPADE_IN3D_INCLUDED 1

#include "yocto/spade/layout.hpp"
#include "yocto/geom/v3d.hpp"

namespace yocto
{
    
    namespace spade
    {
        typedef geom::v3d<ptrdiff_t>     coord3D;
        typedef layout_of<coord3D>       layout3D;
        template <typename T> struct     vertex3D {
            typedef geom::v3d<T> type;
        };

    }
    
}

#endif
