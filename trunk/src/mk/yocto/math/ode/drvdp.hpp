#ifndef YOCTO_MATH_ODE_DRVDP_INCLUDED
#define YOCTO_MATH_ODE_DRVDP_INCLUDED 1

#include "yocto/math/ode/solver.hpp"
#include "yocto/math/ode/rk45.hpp"
#include "yocto/math/ode/rkdp.hpp"

namespace yocto
{
	namespace math
	{
		namespace ode
		{
		
			//! Driver for Runge-Kutta-Dormand-Prince step
			template <typename T>
			struct drvdp
			{
				typedef driver<T,rk45,rkdp> type;
			};
			
		}
	}
}

#endif
