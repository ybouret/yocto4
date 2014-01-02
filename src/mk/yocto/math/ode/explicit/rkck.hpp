#ifndef YOCTO_ODE_RKCK_INCLUDED
#define YOCTO_ODE_RKCK_INCLUDED 1


#include "yocto/math/ode/explicit/step.hpp"

namespace yocto
{
	namespace math
	{
		
		namespace ode
		{
            
            
            //! Runge-Kutta Cash-Karp 4/5 step
            template <typename T>
            class RKCK : public Explicit::step<T>
            {
            public:
                typedef typename field<T>::explicit_type equation;
                typedef typename field<T>::callback_type callback;
                
                explicit RKCK();
                virtual ~RKCK() throw();
                
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
                YOCTO_DISABLE_COPY_AND_ASSIGN(RKCK);
                lw_array<T> &ytmp, &ak2, &ak3, &ak4, &ak5, &ak6;
            };
            
		}
		
	}
	
}

#endif