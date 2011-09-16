#ifndef YOCTO_ODE_RKCK_INCLUDED
#define YOCTO_ODE_RKCK_INCLUDED 1


#include "yocto/math/ode/step.hpp"

namespace yocto
{
	namespace math
	{
		
		namespace ode
		{
			
			//! Runge-Kutta Cash-Karp 4/5 step
			template <typename T>
			class rkck : public step<T>
			{
			public:
				typedef typename field<T>::type equation;
				explicit rkck();
				virtual ~rkck() throw();
				
				virtual void operator()(array<T>        &yout, //-- output
										array<T>        &yerr, //-- embeded error
										equation        &drvs, //-- equation
										const T          x,    //-- starting point
										const T          h,    //-- required step
										const array<T>  &y,    //-- initial value      @x
										const array<T>  &dydx  //-- initial derivative @x
										);
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(rkck);
				lw_array<T> &ytemp, &ak2, &ak3, &ak4, &ak5, &ak6;
			};
		}
		
	}
	
}

#endif
