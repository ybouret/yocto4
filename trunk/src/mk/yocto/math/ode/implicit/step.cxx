#include "yocto/math/ztype.hpp"
#include "yocto/math/ode/implicit/step.hpp"


namespace yocto
{
    namespace math 
    {
        
        namespace ode
        {
            template <> implicit_step<real_t>:: ~implicit_step() throw() {}
            
            template <> implicit_step<real_t>:: implicit_step(const size_t num) :
            Field<real_t>::Arrays(num)
            {}
        }
        
    }
}

