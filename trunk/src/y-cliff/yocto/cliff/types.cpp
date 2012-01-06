#include "yocto/cliff/types.hpp"
#include "yocto/code/swap.hpp"

#include "yocto/exceptions.hpp"
#include <cerrno>

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
		
		
		void layout_base:: split(unit_t      &lo, 
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
		
		
		void layout_base:: inside( unit_t *lo, unit_t *up, const unit_t *w ) const
		{
			for( size_t i=0; i < dimensions; ++i )
			{
				assert( up[i] - lo[i] + 1 == w[i] );
				if( w[i] < 3 )
					throw libc::exception( EDOM, "layout_base::inside( too small in dimension #%u", unsigned(i) );
				++lo[i];
				--up[i];
			}
		}
		
	}
	
}

