#include "yocto/math/ode/solver-data.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/math/types.hpp"

namespace yocto
{
    namespace math 
    {
        
        namespace ode
        {
            
            template <>
            solver_data<real_t>:: ~solver_data() throw() {}
            
            template <>
            solver_data<real_t>:: solver_data() : 
            lw_arrays<real_t, memory_type>( 3 ),
            hmin(0),
            eps( numeric<real_t>:: ftol ),
            TINY( 1e-30 ),
            y(     next_array() ),
            dydx(  next_array() ),
            yscal( next_array() )
            {
                
            }
            
        }
        
    }
    
}
