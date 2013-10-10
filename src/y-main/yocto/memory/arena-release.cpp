#include "yocto/memory/arena.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/code/bswap.hpp"

#include "yocto/memory/chunk.hpp"

#include <cstring>

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
			switch( pReleasing_->whose(p, chunk_size ) )
			{
				case chunk::owned_by_this:
					//-- cached
					break;
					
				case chunk::owned_by_next:
					//-- bisection hi
					pReleasing_ = find_owner_of(p, chunk_size, pReleasing_+1, chunk_last_ );
					break;
					
				case chunk::owned_by_prev:
					//-- bisection lo
					pReleasing_ = find_owner_of(p, chunk_size, chunk_,        pReleasing_ );
					break;
			}
			
			assert( pReleasing_ ); assert( chunk::owned_by_this == pReleasing_->whose(p, chunk_size ) );
			
			//------------------------------------------------------------------
			// Release the block
			//------------------------------------------------------------------			
			++accessible_;
			pReleasing_->release(p, block_size );
			
			//------------------------------------------------------------------
			// check memory status
			//------------------------------------------------------------------
			if( pReleasing_->is_empty() )
			{
				if( pAvailable_ )
				{
					//----------------------------------------------------------
					// we have two empty chunks
					//----------------------------------------------------------
					assert( pAvailable_ != pReleasing_ );
					assert( pAvailable_->is_empty()    );
					
					//----------------------------------------------------------
					//-- choose the highest chunk
					//----------------------------------------------------------
					if( pAvailable_ <= pReleasing_ )
						cswap<chunk *>( pAvailable_, pReleasing_ );
					
					assert( pAvailable_ > pReleasing_ );
					assert( pAvailable_->data > pReleasing_->data );
					
					//----------------------------------------------------------
					// remove to_release
					//----------------------------------------------------------
					chunk *to_release = pAvailable_;
					size_t csz        = chunk_size;
					kind<global>::release_as<uint8_t>( to_release->data, csz );
					memmove( to_release, to_release+1, sizeof(chunk) * static_cast<size_t>( --chunk_last_ - to_release ) );
					--size_;
					accessible_ -= num_blocks;
					
					//----------------------------------------------------------
					//-- update caching
					//----------------------------------------------------------
					if( pAcquiring_ >= to_release )
					{
						
						if( pAcquiring_ > to_release ) 
						{
							/* acquiring was shifted down */
							--pAcquiring_;
						}
						else
						{
							/* acquiring was destroyed ! */
							assert( to_release == pAcquiring_ );
							pAcquiring_ = pReleasing_;
						}
					}
					
					
				}
				
				//--------------------------------------------------------------
				// in any case, the new pAvailable_ is pReleasing_...
				//--------------------------------------------------------------
				assert(pReleasing_->is_empty());
				pAvailable_ = pReleasing_;
				
				
			}
			
		}
		
	}
	
}
