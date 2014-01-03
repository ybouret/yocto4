#ifndef YOCTO_MATH_ODE_EXPLICIT_STEP_INCLUDED
#define YOCTO_MATH_ODE_EXPLICIT_STEP_INCLUDED 1

#include "yocto/math/ode/types.hpp"
#include "yocto/sequence/lw-arrays.hpp"

namespace yocto
{
    namespace math
    {
        
        namespace ode
        {
            
            
            //! memory and method for one step
            template <typename T>
            class explicit_step : public lw_arrays<T,ode::memory_type>
            {
            public:
                typedef typename field<T>::explicit_type equation;
                typedef typename field<T>::callback_type callback;
                
                virtual ~explicit_step() throw();
                
                //! compute evaluation and embedded error
                /**
                 \param yout output
                 \param yerr embedded error
                 \param drvs the differential equation
                 \param x    starting point
                 \param h    guess step
                 \param y    initial values @x
                 \param dydx initial derivatives @x
                 */
                virtual  void operator()(array<T>        &yout,
                                         array<T>        &yerr,
                                         equation        &drvs,
                                         const T          x,
                                         const T          h,
                                         const array<T>  &y,
                                         const array<T>  &dydx,
                                         callback        *cb
                                         ) = 0;
                
                
            protected:
                explicit explicit_step(size_t num_arrays);
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(explicit_step);
                
            };
            
        }
    }
    
}

#endif
