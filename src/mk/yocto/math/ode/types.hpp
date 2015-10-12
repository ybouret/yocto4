#ifndef YOCTO_ODE_TYPES_INCLUDED
#define YOCTO_ODE_TYPES_INCLUDED 1

#include "yocto/functor.hpp"
#include "yocto/sequence/many-arrays.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/math/matrix.hpp"

namespace yocto
{
	
	namespace math
	{
		
		namespace ode
		{
			typedef memory::global memory_type;
			
			template <typename T> struct Field
			{
                typedef array<T>                                            Array;
                typedef matrix<T>                                           Matrix;
                typedef functor<void,TL3(Array&,T,const Array&)>            Equation; //!< derivs( dydx[], x, y[] );
                typedef functor<void,TL4(Array&,Matrix&, T, const Array &)> Jacobian; //!< jacobn( dfdx[], dfdy[][], x, y[] );
                typedef functor<void,TL2(Array&,T)>                         Callback; //!< adjust trials y[]
                typedef many_arrays<T,ode::memory_type>                     Arrays;
			};
			
		}
		
	}
	
}

#endif
