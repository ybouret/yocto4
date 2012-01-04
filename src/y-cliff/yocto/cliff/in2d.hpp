#ifndef YOCTO_CLIFF_IN2D_INCLUDED
#define YOCTO_CLIFF_IN2D_INCLUDED 1

#include "yocto/cliff/layout.hpp"
#include "yocto/geom/v2d.hpp"

namespace yocto
{
	
	namespace cliff
	{
		typedef geom::v2d<unit_t>    coord2D;
		typedef layout<coord2D>      layout2D;
		template <typename U> struct vertex2D 
		{
			typedef geom::v2d<U> type;
		};
	}
}

#endif
