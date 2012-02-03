#include "yocto/net/io/cache.hpp"

namespace yocto
{
	
	namespace network
	{
		
		io_cache:: ~io_cache() throw()
		{
			while( queues.size ) delete queues.query();
		}
		
		io_cache:: io_cache( const size_t bs ) throw() :
		block_size( io_queue::validate(bs) ),
		queues(),
		blocks()
		{
			
		}
		
		
		void io_cache:: collect( io_queue *Q ) throw()
		{
			
			assert(Q);
			assert(Q->next==NULL);
			assert(Q->prev==NULL);
			assert( block_size == Q->block_size );

			//==================================================================
			// collect all blocks
			//==================================================================
			while( Q->send_blocks.size ) blocks.store( Q->send_blocks.pop_back() );
			while( Q->recv_blocks.size ) blocks.store( Q->recv_blocks.pop_back() );
			while( Q->pool_blocks.size ) blocks.store( Q->pool_blocks.query()    );

			//==================================================================
			// cache queue
			//================================================================== 
			queues.store( Q );
		}
		
		io_queue * io_cache:: provide()
		{
			io_queue *Q = queues.size ? queues.query() : new io_queue( block_size );
			
			// TODO: provide some blocks...
			
			return Q;
		}
	
	
	}

}
