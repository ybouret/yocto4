#ifndef YOCTO_ODE_STIFF_DRIVER_RS_INCLUDED
#define YOCTO_ODE_STIFF_DRIVER_RS_INCLUDED 1

#include "yocto/math/ode/implicit/solver.hpp"
#include "yocto/math/ode/implicit/shampine.hpp"

namespace yocto
{
    namespace math 
    {
        namespace ode
        {

            //! Rosenbrock Shampine stiff integrator
            template <typename T>
            struct driverRS
            {
                typedef implicit_driver<T,shampine> type;
            };
            
        }
        
    }
    
}


#endif
