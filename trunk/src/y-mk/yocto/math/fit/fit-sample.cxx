#include "yocto/math/fit/sample.hpp"
#include "yocto/math/ztype.hpp"

namespace yocto
{
	namespace math
	{
		
		namespace fit
		{
			
			template <>
			sample<real_t>:: ~sample() throw() {}
			
			template <>
			sample<real_t>:: sample( const array<real_t> &ax, const array<real_t> &ay, array<real_t> &az) throw() :
			x(ax),
			y(ay),
			z(az),
			D(-1),
			status( fit_failure )
			{}
			
			
		}
		
	}
}