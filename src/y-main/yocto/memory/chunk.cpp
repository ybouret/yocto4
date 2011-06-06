#include "yocto/memory/chunk.hpp"
#include <cstring>
#include "yocto/code/unroll.hpp"

#if !defined(NDEBUG)
#include <iostream>
#endif

namespace yocto
{
	
	namespace memory
	{
		
		chunk:: ~chunk() throw()
		{
#if !defined(NDEBUG)
			if( stillAvailable < providedNumber )
			{
				std::cerr << "**[memory::chunk] missing " << (providedNumber-stillAvailable) << " blocks" << std::endl;
			}
#endif
		}
		
		chunk:: chunk(void  *           data_entry,
					  size_t            block_size,
					  size_t            num_blocks,
					  size_t            chunk_size) throw() :
		data( static_cast<uint8_t *>( data_entry ) ),
		last( data + chunk_size ),
		firstAvailable(0),
		stillAvailable( num_blocks ),
		providedNumber( stillAvailable )
		{
			assert( block_size >  0 );
			assert( num_blocks >=  num_blocks_min );
			assert( num_blocks <=  num_blocks_max );
			assert( chunk_size == block_size * num_blocks );
			
			//-- format the chunk
			uint8_t *q = data;
			for( uint8_t i=0; i != num_blocks; q += block_size ) 
			{
				*q = ++i;
			}
		}
		
		void *chunk:: acquire( size_t block_size ) throw()
		{
			assert(block_size>0);
			assert( stillAvailable > 0 );
			assert( stillAvailable <= providedNumber);
			uint8_t *p = data + ( firstAvailable * block_size );
			firstAvailable = *p;
			--stillAvailable;
			
			memset( p, 0, block_size );			
			return p;
		}
		
		chunk::ownership chunk::whose( const void *addr ) const throw()
		{
			const uint8_t *p = (const uint8_t *)addr;
			return (p < data) ? owned_by_prev : (  (addr >= last) ? owned_by_next : owned_by_this );
		}
		
		
		void chunk:: release( void *addr, size_t block_size ) throw()
		{
			uint8_t *to_release = (uint8_t *)addr;
			
			assert( data != NULL );
			assert( block_size > 0 );
			assert( addr != NULL   );
			assert( owned_by_this == whose(addr) );
			assert( stillAvailable < providedNumber );
			assert( static_cast<ptrdiff_t>(to_release - data) % block_size == 0 );
			
			/** update linking **/
			*to_release    = firstAvailable;
			firstAvailable = static_cast<uint8_t>( (to_release-data) / block_size );
			
			assert( firstAvailable == (to_release-data) / block_size );
			
			/** update status **/
			++stillAvailable;
		}
		
	}
	
}