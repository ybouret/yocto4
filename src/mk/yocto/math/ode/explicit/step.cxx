#include "yocto/math/ztype.hpp"
#include "yocto/math/ode/explicit/step.hpp"


namespace yocto
{
    namespace math
    {
        
        namespace ode
        {
            
            namespace Explicit
            {
                
                template <> step<real_t>:: ~step() throw() {}

                template <>
                step<real_t> :: step( size_t num_arrays ) :
                lw_arrays<real_t,ode::memory_type>(num_arrays)
                {
                }
                
                
                
            }

        }

    }

}

