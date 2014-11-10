#include "yocto/memory/blocks.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/memory/arena.hpp"
#include "yocto/memory/chunk.hpp"

#include "yocto/code/utils.hpp"
#include "yocto/code/bswap.hpp"
#include "yocto/exceptions.hpp"

#include <cerrno>
#include <cstring>


namespace yocto 
{
	namespace memory
	{
		
		blocks:: ~blocks() throw()
		{
			while( count_ > 0 )
			{
				destruct( & arena_[--count_] );
			}
			kind<global>::release_as<arena>(arena_,total_);
			guard_ = NULL;
		}
		
		blocks:: blocks(size_t a_chunk_size ) throw() :
		cache_(NULL),
		arena_(NULL),
		count_(0),
		total_(0),
		guard_(NULL),
		chunk_size_( a_chunk_size )
		{
			
		}
		
		arena *blocks:: search( size_t block_size ) throw()
		{
			assert( arena_ != NULL   );
			assert( count_ > 0       );
			assert( count_ <= total_ );
			assert( arena_ + count_ == guard_     );
			assert( cache_ >= arena_ );
			assert( cache_  < guard_ ); 
			
			arena *lo = arena_;
			arena *hi = guard_;
			--hi;
			
			while( lo <= hi )
			{
				arena *mid = &lo[ static_cast<ptrdiff_t>(hi-lo) >> 1 ];
				if( mid->block_size < block_size )
				{
					lo = mid+1;
				}
				else
                {
					if( mid->block_size > block_size )
					{
						hi = mid-1;
					}
					else 
					{
						return mid;
					}
				}
				
			}
			
			return NULL;
		}
		
		
		void *blocks:: acquire( size_t block_size )
		{
			assert( block_size > 0 );
			
			//------------------------------------------------------------------
			//
			// query the cache
			//
			//------------------------------------------------------------------
			if(cache_ && 
			   (
				cache_->block_size == block_size    || 
				NULL != (cache_ = search(block_size)) 
				) 
			   )
			{
				assert( cache_ );
				assert( cache_->block_size == block_size );
				return cache_->acquire_block();
			}
			else
			{
				//------------------------------------------------------------------
				//
				// no block_size was found: need another arena
				//
				//------------------------------------------------------------------
				assert( NULL == cache_ );
				
				if( count_ >= total_ )
				{
					//----------------------------------------------------------
					// expand arena array
					//----------------------------------------------------------
					size_t new_total = max_of<size_t>(total_<<1,blocks::initial_arenas);
					if( new_total <= total_ )
						throw libc::exception( ERANGE, "memory::blocks new_total overflow@%u", unsigned(total_));
					
					arena *new_arena = kind<global>::acquire_as<arena>(new_total);
					assert(new_arena);assert(new_total>total_);
					memcpy( new_arena, arena_, count_ * sizeof(arena) );
					
					kind<global>::release_as<arena>(arena_,total_);
					
					total_ = new_total;
					arena_ = new_arena;
					guard_ = arena_+count_;
				}
				
				assert( count_ < total_ );
				//--------------------------------------------------------------
				// compute arena parameters
				//--------------------------------------------------------------
				const size_t num_blocks = clamp<size_t>( chunk::num_blocks_min, chunk_size_ / block_size, chunk::num_blocks_max );
				cache_ = new ( &arena_[ count_++ ] ) arena( block_size, num_blocks );
				++guard_;
				
				//--------------------------------------------------------------
				//  order the arenas by increasing block size
				//--------------------------------------------------------------
				arena *prev = cache_;
				while( --prev >= arena_ && prev->block_size >= cache_->block_size )
				{
					assert( prev->block_size > cache_->block_size );
					bswap<arena>( *prev, *cache_ );
					--cache_;
				}
				
				assert( cache_->block_size == block_size );
				return cache_->acquire_block();
			}
			
		}
		
		
		
		void blocks:: release( void *p, size_t block_size ) throw()
		{
			assert( cache_ != NULL || die("unitialized blocks") );
			assert( p      != NULL || die("invalid invalid")    );
			if( cache_->block_size != block_size ) 
				cache_ = search(block_size);
			assert( cache_ != NULL || die("unregistered block_size") );
			cache_->release_block(p);
		}
		
		arena *blocks:: ensure( size_t block_size )
		{
			arena *a = search( block_size );
			if( !a )
			{
				release( acquire(block_size), block_size );
				a = search(block_size);
				assert( a != NULL || die("internal failure"));
			}
			return a;
		}

		
		
	}
	
}

