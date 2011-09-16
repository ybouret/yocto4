#ifndef YOCTO_MATH_ODE_SOLVER_INCLUDED
#define YOCTO_MATH_ODE_SOLVER_INCLUDED 1

#include "yocto/math/ode/control.hpp"

namespace yocto
{
	
	namespace math
	{
		
		namespace ode
		{
			
			template <typename T>
			class solver : public lw_arrays<T,memory_type>
			{
			public:
				typedef typename field<T>::type equation;
				explicit solver();
				virtual ~solver() throw();
				
				void operator()(equation   &drvs,
								control<T> &ctrl,
								step<T>    &forward,
								array<T>   &ystart,
								const T   x1,
								const T   x2,
								T        &h1,
								const T   hmin,
								const T   eps
								);
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(solver);
				lw_array<T> &y, &dydx, &yscal;
			};
			
		}
		
	}
	
}

#endif
