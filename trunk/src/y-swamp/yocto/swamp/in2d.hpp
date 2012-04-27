#ifndef YOCTO_SWAMP_IN2D_INCLUDED
#define YOCTO_SWAMP_IN2D_INCLUDED 1

#include "yocto/swamp/layout.hpp"
#include "yocto/geom/v2d.hpp"

namespace yocto 
{
    namespace swamp
    {
        typedef geom::v2d<unit_t> coord2D;
        typedef layout<coord2D>   layout2D;
    }
}

#endif
