#include "yocto/math/ode/control.hpp"
#include "yocto/math/ztype.hpp"

namespace yocto
{
	
	namespace math
	{
		
		namespace ode
		{

			template <>
			control<real_t>:: ~control() throw() {}
			
			template <>
			control<real_t>:: control( size_t num ) :
			lw_arrays<real_t,memory_type>(num)
			{}
			
		}

	}

}
