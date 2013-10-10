#include "yocto/spade/layout.hpp"
#include "yocto/code/bswap.hpp"

#include "yocto/exceptions.hpp"
#include <cerrno>

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
        
        bool layout::is_inside( const void *coord_addr, const void *lower_addr, const void *upper_addr) const throw()
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
        
        void layout:: split(unit_t      &lo,
                            unit_t      &hi,
                            const unit_t Lo,
                            const unit_t Hi,
                            const size_t rank,
                            const size_t size )
		{
			assert(Lo<=Hi);
			assert(size>0);
			assert(rank<size);
			unit_t W = Hi - Lo + 1;
			if( unit_t(size) > W )
				throw libc::exception( EDOM, "layout_base::split(size=%u>#divisions=%u)", unsigned(size), unsigned(W));
			unit_t todo = W/size; // first packet
			lo = Lo;              // first offset
			for( size_t i=1; i <= rank; ++i )
			{
				lo += todo;        // update offset
				W  -= todo;        // decrease width
				todo = W/(size-i); // next packet
			}
			hi = lo + (todo-1);
			
		}
        
        
    }
    
}

