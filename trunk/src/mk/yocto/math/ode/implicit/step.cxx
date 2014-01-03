#include "yocto/math/ztype.hpp"
#include "yocto/math/ode/stiff-step.hpp"


namespace yocto
{
    namespace math 
    {
        
        namespace ode
        {
            template <> stiff_step<real_t>:: ~stiff_step() throw() {}
            template <> stiff_step<real_t>:: stiff_step(const size_t num) :
            lw_arrays<real_t, memory_type>(num)
            {}
        }
        
    }
}

