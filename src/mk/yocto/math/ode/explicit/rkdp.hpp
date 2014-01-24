#ifndef YOCTO_MATH_ODE_EXPLICIT_RKDP_INCLUDED
#define YOCTO_MATH_ODE_EXPLICIT_RKDP_INCLUDED 1


#include "yocto/math/ode/explicit/step.hpp"

namespace yocto
{
	namespace math
	{
		
		namespace ode
		{
			
			//! Runge-Kutta Dormand-Price 4/5 step
			template <typename T>
			class RKDP : public explicit_step<T>
			{
			public:
				typedef typename Field<T>::Equation equation;
                typedef typename Field<T>::Callback callback;
                
				explicit RKDP();
				virtual ~RKDP() throw();
				
                virtual void operator()(array<T>        &yout,
                                        array<T>        &yerr,
                                        equation        &drvs,
                                        const T          x,
                                        const T          h,
                                        const array<T>  &y,
                                        const array<T>  &dydx,
                                        callback        *cb
                                        );
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(RKDP);
				array<T> &ytmp, &ak2, &ak3, &ak4, &ak5, &ak6, &ak7;
			};
		}
		
	}
	
}

#endif
