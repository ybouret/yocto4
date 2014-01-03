#ifndef YOCTO_ODE_STIFF_DRIVER_KR_INCLUDED
#define YOCTO_ODE_STIFF_DRIVER_KR_INCLUDED 1

#include "yocto/math/ode/implicit/solver.hpp"
#include "yocto/math/ode/implicit/kaps-rentrop.hpp"

namespace yocto
{
    namespace math 
    {
        namespace ode
        {
            
            //! Rosenbrock Kaps-Rentrop stiff integrator
            template <typename T>
            struct driverKR
            {
                typedef implicit_driver<T,kaps_rentrop> type;
            };
            
        }
        
    }
    
}


#endif
