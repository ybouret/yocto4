
#include "yocto/math/ode/rk45.hpp"
#include "yocto/math/ztype.hpp"

namespace yocto
{
	
	namespace math
	{
		
		namespace ode
		{
			
			template <>
			rk45<real_t>:: ~rk45() throw()
			{
			}
			
			template <>
			rk45<real_t>:: rk45() :
			control<real_t>(2),
			yerr( next_array() ),
			ytemp( next_array() )
			{
			}
			
		}

	}

}
