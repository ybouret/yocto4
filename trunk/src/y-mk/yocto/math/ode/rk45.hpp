#ifndef YOCTO_MATH_ODE_RK45_INCLUDED
#define YOCTO_MATH_ODE_RK45_INCLUDED 1

#include "yocto/math/ode/control.hpp"


namespace yocto
{
	
	namespace math
	{
		
		namespace ode
		{
				
			template <typename T>
			class rk45 : public control<T>
			{
			public:
				explicit rk45();
				virtual ~rk45() throw();
				
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(rk45);
				lw_array<T> &yerr,&ytemp;
			};
		}

	}

}
#endif
