#include "yocto/math/ztype.hpp"
#include "yocto/math/ode/explicit/step.hpp"


namespace yocto
{
    namespace math
    {
        
        namespace ode
        {
            
            
            template <> explicit_step<real_t>:: ~explicit_step() throw() {}
            
            template <>
            explicit_step<real_t> :: explicit_step( size_t num_arrays ) :
            lw_arrays<real_t,ode::memory_type>(num_arrays)
            {
            }
            
            
            
        }
        
    }
    
}

