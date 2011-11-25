#include "yocto/spade/types.hpp"
#include "yocto/code/round.hpp"
#include "yocto/code/swap.hpp"
//#include <iostream>

namespace yocto
{
	
	namespace spade
	{
		
		layout_base::  layout_base() throw() {}
		layout_base:: ~layout_base() throw() {}
		
		size_t layout_base:: order(const void *umin,
								   const void *umax, 
								   size_t      ndim,
								   const void *ulen) throw()
		{
			
			assert( umin != NULL );
			assert( umax != NULL );
			assert( ndim >= 1 );
			assert( ndim <= 3 );
			assert( ulen != NULL );
			
			size_t  items = 1;
			
			unit_t *amin = (unit_t *) umin;
			unit_t *amax = (unit_t *) umax;
			unit_t *alen = (unit_t *) ulen;
			for( size_t i=0; i < ndim; ++i, ++amin, ++amax, ++alen )
			{
				if( *amin > *amax ) 
					cswap<unit_t>( *amin, *amax );
				items *= ( *alen = 1 + *amax - *amin );
			}
			return items;
			
			
		}
		
		
		size_t layout_base:: count( const void *ulen, size_t ndim ) throw()
		{
			assert( ndim >= 1 );
			assert( ndim <= 3 );
			assert( ulen != NULL );
			
			size_t  items = 1;
			unit_t *alen = (unit_t *) ulen;
			for( size_t i=0; i < ndim; ++i, ++alen )
			{
				assert( *alen > 0 );
				items *= *alen;
			}
			return items;
		}
		
		void   layout_base::split1(unit_t &L,
								   unit_t &R,
								   size_t  world_size, 
								   size_t  world_rank ) throw()
		{
			assert( world_size > 0 );
			assert( world_rank < world_size );
			
			
			unit_t N = 1 + R - L;      // total remaining size
			unit_t W = N / world_size; // first width
			
			// todo: check i=1 or i=0 ?
			for( size_t i=1; i <= world_rank; ++i )
			{
				L += W; // increase offset
				N -= W; // decrease toto remaining size
				W  = N / (world_size - i); // next offset				
			}
			R = L + W -1 ;
		}
		
		
		void layout_base:: update( void * & buffer, const size_t lengths ) throw()
		{
			assert( buffer );
			
			uint8_t *p = (uint8_t *) buffer;
			buffer    = & p[ lengths ];
		}
		
		size_t layout_base:: len2off( size_t lengths[], size_t ndim ) throw()
		{
			assert( lengths );
			assert( ndim >= 1 );
			assert( ndim <= 3 );
			size_t old = 0;
			size_t ans = 0;
			for( size_t i=0; i < ndim; ++i )
			{
				//std::cerr << "\t+length[" << i << "]=" << lengths[i] << std::endl;
				ans += lengths[i];
				lengths[i] = old;
				//std::cerr << "\t\t--> " << old << std::endl;
				old = ans;
			}
			ans = YOCTO_MEMALIGN(ans);
			//std::cerr << "\t|=>total=" << ans << std::endl;
			return ans;
		}
		
	}
}
