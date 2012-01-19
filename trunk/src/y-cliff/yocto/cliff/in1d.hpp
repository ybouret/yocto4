#ifndef YOCTO_CLIFF_IN1D_INCLUDED
#define YOCTO_CLIFF_IN1D_INCLUDED 1

#include "yocto/cliff/region.hpp"

namespace yocto
{
	
	namespace cliff
	{
		typedef unit_t               coord1D;
		typedef layout<coord1D>      layout1D;	
		template <typename U> struct vertex1D { typedef U type; };
		template <typename U> struct region1D { typedef region<U,vertex1D> type; };
	}
}

#endif
