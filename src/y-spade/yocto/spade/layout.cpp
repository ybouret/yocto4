#include "yocto/spade/layout.hpp"
#include "yocto/code/swap.hpp"

namespace yocto
{
    
    namespace spade
    {
        
        layout:: ~layout() throw() {}
        
        layout:: layout( size_t num_dim ) throw() : dimensions(num_dim)
        {
            assert(dimensions>0);
        }
        
        size_t   layout:: setup(const void *lower_addr,
                                const void *upper_addr,
                                const void *pitch_addr,
                                const void *width_addr) const throw()
        {
            unit_t *lower = (unit_t *)lower_addr;
            unit_t *upper = (unit_t *)upper_addr;
            unit_t *width = (unit_t *)width_addr;
            unit_t *pitch = (unit_t *)pitch_addr;
            
            size_t ans = 1;
            for( size_t i=0; i < dimensions; ++i )
            {
                unit_t &L = lower[i];
                unit_t &U = upper[i];
                unit_t &W = width[i];
                if( L > U ) cswap(L,U);  assert(L<=U);
                W=(U+1)-L;               assert(W>0);
                ans *= W;
            }
            
            pitch[0] = 1;
            for( size_t i=1; i < dimensions; ++i )
            {
                pitch[i] = pitch[i-1] * width[i-1];
            }
            return ans;
        }
        
    }
    
}