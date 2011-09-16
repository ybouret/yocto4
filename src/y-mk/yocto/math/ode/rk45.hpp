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
				typedef typename field<T>::type equation;
				
				explicit rk45();
				virtual ~rk45() throw();
				
				virtual void operator()(step<T>        &forward,
										array<T>       &y, 
										const array<T> &dydx,
										equation       &drvs,
										T              &x,
										const T         h_try,
										T              &h_did,
										T              &h_next,
										const array<T> &yscal,
										const T         eps);
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(rk45);
				lw_array<T> &yerr,&ytemp;
			};
		}
		
	}
	
}
#endif
