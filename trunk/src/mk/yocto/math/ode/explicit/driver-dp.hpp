#ifndef YOCTO_MATH_ODE_EXPLICIT_DRIVER_DP_INCLUDED
#define YOCTO_MATH_ODE_EXPLICIT_DRIVER_DP_INCLUDED 1

#include "yocto/math/ode/explicit/solver.hpp"
#include "yocto/math/ode/explicit/rk45.hpp"
#include "yocto/math/ode/explicit/rkdp.hpp"

namespace yocto
{
    namespace math
    {
        namespace ode
        {
            template <typename T>
            struct driverDP
            {
                typedef explicit_driver<T,RK45,RKDP> type;
            };
        }
    }
}
#endif
