#include "yocto/math/ode/step.hpp"
#include "yocto/math/ztype.hpp"

namespace yocto
{
	
	namespace math
	{
		namespace ode
		{
			
			template <>
			step<real_t>:: ~step() throw() {}
			
			template <>
			step<real_t>:: step( size_t num ) :
			lw_arrays<real_t,memory_type>( num )
			{}
			
			
		}
	}
}
