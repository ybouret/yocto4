#ifndef YOCTO_SPADE_IN2D_INCLUDED
#define YOCTO_SPADE_IN2D_INCLUDED 1

#include "yocto/spade/layout.hpp"
#include "yocto/geom/v2d.hpp"

namespace yocto
{
	
	namespace spade
	{
		
		typedef geom::v2d<unit_t> coord2D;
		typedef layout<coord2D>   layout2D;
		
		
	}
	
}

#endif
