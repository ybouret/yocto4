#ifndef YOCTO_SWAMP_IN3D_INCLUDED
#define YOCTO_SWAMP_IN3D_INCLUDED 1

#include "yocto/swamp/layout.hpp"
#include "yocto/geom/v3d.hpp"

namespace yocto 
{
    namespace swamp
    {
        typedef geom::v3d<unit_t> coord3D;
        typedef layout<coord3D>   layout3D;
    }
}

#endif
