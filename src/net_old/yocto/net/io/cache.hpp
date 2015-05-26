#ifndef YOCTO_NET_IO_CACHE_INCLUDED
#define YOCTO_NET_IO_CACHE_INCLUDED 1

#include "yocto/net/io/block.hpp"
#include "yocto/core/pool.hpp"

namespace yocto
{
	
	namespace network
	{
		
        //! manage io_blocks with common block_size
		class io_cache
		{
		public:
			explicit io_cache( const size_t bs ) throw();
			virtual ~io_cache() throw();
			
			const size_t block_size;
			
			io_block *provide();
			void      collect( io_block *block ) throw();
			
			size_t count() const throw();
			size_t bytes() const throw();
			
			
		private:
			core::pool_of<io_block> blocks;
			YOCTO_DISABLE_COPY_AND_ASSIGN(io_cache);
		};
		
	}
	
}

#endif
