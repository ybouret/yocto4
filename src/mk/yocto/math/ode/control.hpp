#ifndef YOCTO_MATH_ODE_CONTROL_INCLUDED
#define YOCTO_MATH_ODE_CONTROL_INCLUDED 1

#include "yocto/math/ode/step.hpp"


namespace yocto
{

	namespace math
	{
		
		namespace ode
		{
			
			//! control algorithm, depends on step order
			template <typename T>
			class control : public lw_arrays<T,memory_type>
			{
			public:
				typedef typename field<T>::type equation;
				virtual ~control() throw();
				
				virtual void operator()(step<T>        &forward,
										array<T>       &y, 
										const array<T> &dydx,
										equation       &drvs,
										T              &x,
										const T         h_try,
										T              &h_did,
										T              &h_next,
										const array<T> &yscal,
										const T         eps) = 0;
										
				
			protected:
				explicit control( size_t num );
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(control);
			};
			
		}
	}
	
}


#endif
