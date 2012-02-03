#ifndef YOCTO_NET_IO_CACHE_INCLUDED
#define YOCTO_NET_IO_CACHE_INCLUDED 1

#include "yocto/net/io/queue.hpp"

namespace yocto
{
	
	namespace network
	{
		
		class io_cache
		{
		public:
			explicit io_cache( const size_t bs ) throw();
			virtual ~io_cache() throw();
			
			const size_t block_size;
			
			io_queue *provide();
			void      collect( io_queue * ) throw();
			
			
		private:
			core::pool_of<io_queue> queues;
			core::pool_of<io_block> blocks;
			YOCTO_DISABLE_COPY_AND_ASSIGN(io_cache);
		};
		
	}
	
}

#endif
