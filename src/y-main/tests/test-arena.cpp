#include "yocto/utest/run.hpp"
#include "yocto/memory/arena.hpp"
#include "yocto/code/round.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/wtime.hpp"
#include "yocto/memory/global.hpp"

using namespace yocto;
using namespace memory;


YOCTO_UNIT_TEST_IMPL(arena)
{
	size_t num  = 500;
	void **addr = kind<global>::acquire_as<void*>(num);
	
	wtime chrono;
	for( size_t block_size=1; block_size <= 20; block_size += ( 1 + alea_less_than<size_t>(4) ) )
	{
		std::cerr << "block_size =" << block_size << std::endl;
		for( size_t num_blocks = chunk::num_blocks_min; 
			num_blocks        <= chunk::num_blocks_max; 
			num_blocks += ( 1 + alea_less_than<size_t>(4) ))
		{
			double tmx = 0;
			arena A(block_size,num_blocks);
			chrono.start();
			for( size_t i=0; i < num; ++i )
			{
				addr[i] = A.acquire_block();
			}
			tmx += chrono.query();
			
			const size_t nfree = num/2;
			
			for( size_t iter=0; iter<2; ++iter)
			{
				c_shuffle( addr, num );
				chrono.start();
				for( size_t i=0; i < nfree; ++i )
				{
					A.release_block( addr[i] );
				}
				for( size_t i=0; i < nfree; ++i )
				{
					addr[i] = A.acquire_block();
				}
				tmx += chrono.query();
			}
			c_shuffle(addr, num);
			
			chrono.start();
			for( size_t i=0; i < num; ++i )
			{
				A.release_block( addr[i] );
			}
			tmx += chrono.query();
			std::cout << block_size << " " << num_blocks << " " << tmx/num << std::endl;
		}
	}
	
	kind<global>::release_as<void*>( addr, num );
	
}
YOCTO_UNIT_TEST_DONE()
