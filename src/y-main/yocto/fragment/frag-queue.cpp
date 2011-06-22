#include "yocto/fragment/queue.hpp"
#include "yocto/object.hpp"

namespace yocto
{
	
	namespace fragment
	{
		
		
		queue::queue(size_t user_block_size) throw() :
		layout( user_block_size, __log2 ),
		bytes_(0),
		list_(),
		pool_()
		{
			
		}
		
		queue:: ~queue() throw() { release(); }
		
		void queue:: free() throw()
		{
			while( list_.size )
			{
				pool_.store( list_.pop_back() );
			}
			bytes_ = 0;
		}
		
		void queue:: release() throw()
		{
			while( list_.size )
			{
				block *blk = list_.pop_back();
				destruct(blk);
				object::release1<block>(blk);
			}
			
			pool_.delete_with( object::release1<block> );
			bytes_ = 0;
		}
		
		void  queue:: dispatch( void *frag_data, size_t frag_size, void *args)
		{
			queue *q   = (queue *)args;
			block *blk = q->list_.tail;
			assert( frag_size <= q->block_size );
			if( blk && blk->unused() >= frag_size )
			{
				
			}
			else 
			{
				
			}
			
		}
		
		size_t queue:: write( const void *buffer, size_t buflen ) 
		{
			assert(!(buffer==NULL&&buflen>0) );
			const size_t num_blocks = list_.size;
			try {
				process((void*)buffer, buflen, dispatch, this);
				bytes_ += buflen;
			}
			catch (...) {
				while( list_.size > num_blocks )
				{
					pool_.store( list_.pop_back() );
				}
				throw;
			}
			return 0;
		}
		
		
	}
	
}
