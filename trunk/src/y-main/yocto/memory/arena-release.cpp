#include "yocto/memory/arena.hpp"

#include <iostream>

namespace yocto
{
	
	namespace memory
	{
		
		// bissection to find releasing chunk
		static inline chunk *find_owner_of(void        *p, 
										   const size_t chunk_size,
										   chunk       *lo, //-- first valid
										   chunk       *hi  //-- first invalid
										   ) throw()
		{
			--hi;
			
			for(;;)
			{
				assert(hi>=lo);
				chunk *mid = &lo[ static_cast<ptrdiff_t>(hi-lo) >> 1 ];
				switch( mid->whose(p, chunk_size) )
				{
					case chunk::owned_by_this:
						return mid;
						
					case chunk::owned_by_next:
						lo = mid+1;
						break;
						
					case chunk::owned_by_prev:
						hi = mid-1;
						break;
				}
			}
			return NULL;
		}
		
		void  arena:: release_block( void *p ) throw()
		{
			assert( chunk_ != NULL );
			assert( size_ >  0 );
			assert( chunk_last_ == chunk_ + size_ );
			assert( pReleasing_ != NULL );
			assert( pAcquiring_ != NULL );
			
			//------------------------------------------------------------------
			// Find The Owner
			//------------------------------------------------------------------
			switch( pReleasing_->whose(p, chunk_size_ ) )
			{
				case chunk::owned_by_this:
					//-- cached
					break;
					
				case chunk::owned_by_next:
					//-- bisection hi
					pReleasing_ = find_owner_of(p, chunk_size_, pReleasing_+1, chunk_last_ );
					break;
					
				case chunk::owned_by_prev:
					//-- bisection lo
					pReleasing_ = find_owner_of(p, chunk_size_, chunk_,        pReleasing_ );
					break;
			}
			
			assert( pReleasing_ ); assert( chunk::owned_by_this == pReleasing_->whose(p, chunk_size_ ) );
			//------------------------------------------------------------------
			// Release the block
			//------------------------------------------------------------------			
			++accessible_;
			pReleasing_->release(p, block_size_);
			
			if( pReleasing_->is_empty() )
			{
				if( pAvailable_ )
				{
					assert( pAvailable_ != pReleasing_ );
					std::cerr << "Two Chunks are Empty !" << std::endl;
					exit(-1);
				}
				else 
				{
					pAvailable_ = pReleasing_;
				}
				
			}
			
		}
		
	}
	
}
