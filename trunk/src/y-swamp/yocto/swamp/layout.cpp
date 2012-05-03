#include "yocto/swamp/layout.hpp"
#include "yocto/code/swap.hpp"


namespace yocto 
{
    namespace swamp
    {
        layout_base:: ~layout_base() throw() {}
        
        layout_base:: layout_base( size_t num_dims ) throw() :
        dimensions( num_dims )
        {
            
        }
        
        size_t   layout_base:: setup(const void *lower_addr,
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
        
        
        bool layout_base::is_inside( const void *coord_addr, const void *lower_addr, const void *upper_addr) const throw()
        {
            const unit_t *lower = (unit_t *)lower_addr;
            const unit_t *upper = (unit_t *)upper_addr;
            const unit_t *coord = (unit_t *)coord_addr;
            for( size_t i=0; i < dimensions; ++i )
            {
                const unit_t coord_i = coord[i];
                if( coord_i < lower[i] || coord_i > upper[i] )
                    return false;
            }
            return true;
        }
        
    }
}
