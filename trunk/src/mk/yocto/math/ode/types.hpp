#ifndef YOCTO_ODE_TYPES_INCLUDED
#define YOCTO_ODE_TYPES_INCLUDED 1

#include "yocto/functor.hpp"
#include "yocto/sequence/array.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/math/kernel/matrix.hpp"

namespace yocto
{
	
	namespace math
	{
		
		namespace ode
		{
			typedef memory::global memory_type;
			
			template <typename T> struct field
			{
				//! derivs( dydx[], x, y[] )
				typedef functor<void,TL3(array<T>&,T,const array<T>&)> type;
                
                typedef functor<void,TL3(array<T>&,T,const array<T>&)>                 explicit_type; //!< derivs( dydx[], x, y[] );
                typedef functor<void,TL4(array<T> &, matrix<T> &, T, const array<T>&)> implicit_type; //!< jacobn( dfdx[], dfdy[][], x, y[] );
                typedef functor<void,TL2(array<T> &,T)>                                callback_type; //!< adjust trials y[]
                
                //! jacobn( dfdx[], dfdy[][], x, y[] );
				typedef functor<void,TL4(array<T> &, matrix<T> &, T, const array<T>&)> diff;
			};
			
		}
		
	}
	
}

#endif
