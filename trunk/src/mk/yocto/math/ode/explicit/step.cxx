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
            Field<real_t>::LW_Arrays(num_arrays)
            {
            }
            
            
            
        }
        
    }
    
}

