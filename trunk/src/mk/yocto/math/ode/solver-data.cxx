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
            solver_data<real_t>:: solver_data( real_t user_eps ) : 
            Field<real_t>::Arrays( 3 ),
            hmin(0),
            eps( user_eps ),
            TINY( REAL(1e-30) ),
            y(     next_array() ),
            dydx(  next_array() ),
            yscal( next_array() )
            {
                
            }
            
        }
        
    }
    
}
