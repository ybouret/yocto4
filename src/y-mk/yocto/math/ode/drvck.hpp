#ifndef YOCTO_MATH_ODE_DRVCK_INCLUDED
#define YOCTO_MATH_ODE_DRVCK_INCLUDED 1

#include "yocto/math/ode/solver.hpp"
#include "yocto/math/ode/rk45.hpp"
#include "yocto/math/ode/rkck.hpp"

namespace yocto
{
	namespace math
	{
		namespace ode
		{
		
			//! Driver for Runge-Kutta-Cash-Karp step
			template <typename T>
			struct drvck
			{
				typedef driver<T,rk45,rkck> type;
			};
			
		}
	}
}

#endif
