#include "yocto/memory/slab.hpp"
#include "yocto/code/round.hpp"

#if !defined(NDEBUG)
#include <iostream>
#endif

#include <cstring>

namespace yocto
{
	
	namespace memory
	{
		
		slab:: ~slab() throw() {
#if !defined(NDEBUG)
			if( stillAvailable < numBlocks )
			{
				std::cerr << "[memory::slab] missing " << numBlocks - stillAvailable << ", blockSize=" << blockSize << std::endl;
			}
#endif
		}
		
		
		size_t slab:: round_bs( size_t block_size ) throw()
		{
			assert(block_size>0);
			return YOCTO_ROUND(bs_round_ln2,block_size);
		}
		
		
        
		size_t slab:: bytes_for( size_t block_size, size_t num_blocks )
		{
			return round_bs(block_size) * num_blocks;
		}
		
		
		slab:: slab( void *entry, size_t block_size, size_t num_blocks ) throw() :
		data( static_cast<uint8_t*>(entry) ),
		firstAvailable(0),
		stillAvailable(num_blocks),
		blockSize( round_bs(block_size) ),
		numBlocks( num_blocks )
		{
			assert( blockSize == round_bs(blockSize) );
			assert( blockSize % sizeof(size_t) == 0 );
			
			link();
		}
		
        
        void slab:: reset() throw()
        {
            firstAvailable = 0;
            stillAvailable = numBlocks;
            link();
        }
        
        void slab:: format(void        *entry,
                           const size_t block_size,
                           const size_t num_blocks ) throw()
        {
            data = static_cast<uint8_t*>(entry);
            (size_t &)numBlocks = num_blocks;
            (size_t &)blockSize = block_size;
            assert( blockSize == round_bs(blockSize) );
			assert( blockSize % sizeof(size_t) == 0 );
            reset();
        }
        
        
        void slab:: link() throw()
        {
            assert(0==firstAvailable);
            assert(numBlocks==stillAvailable);
            
            uint8_t     *q = data;
			for( size_t i=0; i < numBlocks; q += blockSize )
			{
				*(size_t *)q = ++i;
			}
        }
        
		
		void *slab:: acquire() throw()
		{
			assert( stillAvailable > 0 );
			assert( stillAvailable <= numBlocks);
			
			uint8_t     *p = data + ( firstAvailable * blockSize );
			firstAvailable = *(size_t *)p;
			--stillAvailable;
			
			memset( p, 0, blockSize );
			return p;
			
		}
		
		void slab:: release( void *addr ) throw()
		{
			uint8_t *to_release = (uint8_t *)addr;
			assert( to_release>=data);
			assert( to_release<data+(numBlocks*blockSize));
			assert( data != NULL );
			assert( addr != NULL   );
			assert( stillAvailable < numBlocks );
			assert( static_cast<ptrdiff_t>(to_release - data) % blockSize == 0 );
			
			/** update linking **/
			*(size_t *)to_release = firstAvailable;
			firstAvailable        = static_cast<size_t>( (to_release-data) / blockSize );
			
			assert( firstAvailable == (to_release-data) / blockSize );
			
			/** update status **/
			++stillAvailable;
			
		}
		
		
		
		
	}
	
}
