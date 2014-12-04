#ifndef YOCTO_MATH_ODE_EXPLICIT_STEP_INCLUDED
#define YOCTO_MATH_ODE_EXPLICIT_STEP_INCLUDED 1

#include "yocto/math/ode/types.hpp"

namespace yocto
{
    namespace math
    {
        
        namespace ode
        {
            
            
            //! memory and method for one step
            template <typename T>
            class explicit_step : public Field<T>::Arrays
            {
            public:
                typedef typename Field<T>::Equation equation;
                typedef typename Field<T>::Callback callback;
                
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
                 \param cb   called BEFORE the equation on a trial point/time
                 The callback is used to modify (i.e. project or something)
                 the trial vectors.
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
                explicit explicit_step(size_t na);
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(explicit_step);
                
            };
            
        }
    }
    
}

#endif
