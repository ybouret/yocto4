#include "yocto/utest/run.hpp"
#include "yocto/memory/chunk.hpp"
#include "yocto/code/round.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/sys/wtime.hpp"

using namespace yocto;
using namespace memory;


YOCTO_UNIT_TEST_IMPL(chunk)
{
	
	wtime chrono;
	std::cerr << "sizeof(chunk)=" << sizeof(chunk) << std::endl;
	void *addr[ chunk::num_blocks_max ];
	double ell = 0;
	size_t naq = 0;
	for( size_t block_size=1; block_size <= 40; block_size += (1+alea_lt(4)) )
	{
		std::cerr << "block_size=" << block_size << std::endl;
		for( size_t num_blocks = chunk::num_blocks_min; num_blocks <= chunk::num_blocks_max; num_blocks += (1+alea_lt(4)) )
		{
			const size_t chunk_size = block_size * num_blocks;
			void        *data_entry = operator new( chunk_size );
			uint64_t     wksp[ YOCTO_U64_FOR_ITEM(chunk) ];
			
			{
				
				chunk *ch = new(wksp) chunk(data_entry,block_size,num_blocks,chunk_size);
				chrono.start();
				for(register size_t i=0; i < num_blocks; ++i)
					addr[i] = ch->acquire(block_size);
				ell += chrono.query();
				naq += num_blocks;
				
				const size_t nfree = num_blocks/2;
				
				c_shuffle(addr,num_blocks);
				for( register size_t i=0; i < nfree; ++i )
				{
					ch->release( addr[i], block_size );
				}
				chrono.start();
				for( register size_t i=0; i < nfree; ++i )
				{
					addr[i] =  ch->acquire(block_size);
				}
				ell += chrono.query();
				naq += nfree;
				c_shuffle(addr, num_blocks);
				
				
				for( register size_t i=0; i < num_blocks; ++i )
				{
					ch->release( addr[i], block_size);
				}
				destruct(ch);
			}
			
			operator delete( data_entry );
		}
	}
	std::cerr << "<acquire time>=" << ell / naq << std::endl;
	
}
YOCTO_UNIT_TEST_DONE()
