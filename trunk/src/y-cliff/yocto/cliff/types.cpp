#include "yocto/cliff/types.hpp"
#include "yocto/code/swap.hpp"

namespace yocto
{
	
	namespace cliff
	{
		
		layout_base:: layout_base( size_t ndim ) throw() :
		dimensions(ndim)
		{
			assert(dimensions>0);
		}
		
		
		layout_base:: ~layout_base() throw()
		{
			
		}
		
		
		size_t layout_base:: setup( const void *lo, const void *hi, const void *w )
		{
			unit_t *L = (unit_t *)lo;
			unit_t *H = (unit_t *)hi;
			unit_t *W = (unit_t *)w;
			
			size_t ans = 1;
			for( size_t i=0; i < dimensions; ++i )
			{
				if( L[i] > H[i] ) cswap( L[i], H[i]);
				assert( L[i] <= H[i] );
				ans *= ( W[i] = 1+H[i]-L[i]);
			}
			return ans;
		}

		
	}
	
}