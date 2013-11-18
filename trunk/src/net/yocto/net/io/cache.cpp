#include "yocto/net/io/cache.hpp"
#include "yocto/code/round.hpp"

namespace yocto
{
	
	namespace network
	{
		
		io_cache:: ~io_cache() throw()
		{
			while( blocks.size ) delete blocks.query();
		}
		
		static inline size_t __check_bs( const size_t bs ) throw()
		{
			return bs < 16 ? 16 : YOCTO_ROUND16(bs) ;
		}
		
		io_cache:: io_cache( const size_t bs ) throw() :
		block_size( __check_bs(bs) ),
		blocks()
		{
			
		}
		
		io_block * io_cache:: provide( )
		{
			if( blocks.size <= 0 )
			{
				return new io_block( block_size );
			}
			else
			{
				io_block *blk = blocks.query();
				blk->clear();
				return blk;
			}
		}
		
		void io_cache:: collect( io_block *blk ) throw()
		{
			assert( blk );
			assert( NULL == blk->next );
			assert( NULL == blk->prev );
			blocks.store( blk );
		}
		
		size_t io_cache:: count() const throw() { return blocks.size; }
		size_t io_cache:: bytes() const throw() { return blocks.size * block_size; }
		
			
	
	}

}
