#ifndef YOCTO_ODE_TYPES_INCLUDED
#define YOCTO_ODE_TYPES_INCLUDED 1

#include "yocto/functor.hpp"
#include "yocto/sequence/array.hpp"

namespace yocto
{
	
	namespace math
	{
	
		template <typename T> ode
		{
			//! field( dydx[], x, y[] )
			typedef functor<void,TL3(array<T>&,T,const array<T>&)> field;
			
		};
		
	}
	
}

#endif
