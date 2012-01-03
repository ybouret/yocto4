#ifndef YOCTO_CLIFF_IN3D_INCLUDED
#define YOCTO_CLIFF_IN3D_INCLUDED 1

#include "yocto/cliff/layout.hpp"
#include "yocto/geom/v3d.hpp"

namespace yocto
{
	
	namespace cliff
	{
		typedef geom::v3d<unit_t> coord3D;
		typedef layout<coord3D>   layout3D;		
	}
}

#endif
