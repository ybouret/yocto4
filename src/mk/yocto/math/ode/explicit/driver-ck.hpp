#ifndef YOCTO_MATH_ODE_EXPLICIT_DRIVER_CK_INCLUDED
#define YOCTO_MATH_ODE_EXPLICIT_DRIVER_CK_INCLUDED 1

#include "yocto/math/ode/explicit/solver.hpp"
#include "yocto/math/ode/explicit/rk45.hpp"
#include "yocto/math/ode/explicit/rkck.hpp"

namespace yocto
{
    namespace math
    {
        namespace ode
        {
            template <typename T>
            struct driverCK
            {
                typedef explicit_driver<T,RK45,RKCK> type;
            };
        }
    }
}
#endif
