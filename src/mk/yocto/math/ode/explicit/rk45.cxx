#include "yocto/math/ztype.hpp"
#include "yocto/math/ode/explicit/rk45.hpp"


namespace yocto
{
    namespace math
    {
        namespace ode
        {
            template <> rk45<real_t>:: ~rk45() throw() {}
            
            
        }
    }
}
