#ifndef YOCTO_SWAMP_IN2D_INCLUDED
#define YOCTO_SWAMP_IN2D_INCLUDED 1

#include "yocto/swamp/layout.hpp"
#include "yocto/geom/v2d.hpp"
#include "yocto/swamp/region.hpp"

namespace yocto 
{
    namespace swamp
    {
        typedef geom::v2d<unit_t>    coord2D;
        typedef layout<coord2D>      layout2D;
        template <typename U> struct vertex2D { typedef geom::v2d<U> type; };
        template <typename U> struct region2D { typedef region<U,vertex2D> type; };

    }
}

#endif
