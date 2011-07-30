#ifndef YOCTO_SPADE_IN3D_INCLUDED
#define YOCTO_SPADE_IN3D_INCLUDED 1

#include "yocto/spade/layout.hpp"
#include "yocto/geom/v3d.hpp"

namespace yocto
{
	
	namespace spade
	{
		
		typedef geom::v3d<unit_t>  coord3D;
		typedef layout<coord3D>    layout3D;
		
		
	}
	
}

#endif
