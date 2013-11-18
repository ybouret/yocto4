#include "yocto/utest/run.hpp"
#include "yocto/memory/slab.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;
using namespace memory;


YOCTO_UNIT_TEST_IMPL(slab)
{
	const size_t nb_max = 500;
	void        *addr[nb_max];
	
	for( size_t bs=1; bs <= 100; bs += (1 + alea_lt(16) ) )
	{
		std::cerr << "bs=" << bs << " => " << slab::round_bs(bs) << std::endl;
		
		for( size_t num_blocks=0; num_blocks < nb_max; num_blocks += (1 + alea_lt(16) ) )
		{
			//(std::cerr << ".").flush();
			size_t block_size = bs;
			size_t bytes      = slab::bytes_for( block_size, num_blocks );
			void  *entry      = kind<global>::acquire( bytes );
			
			slab S( entry, block_size, num_blocks );
			
			for( size_t i=0; i < num_blocks; ++i )
			{
				addr[i] =  S.acquire();
			}
			
			const size_t nfree = num_blocks/2;
			for( size_t k=0; k < 2; ++k )
			{
				c_shuffle(addr, num_blocks);
				for( size_t i=0; i < nfree; ++i ) S.release( addr[i] );
				for( size_t i=0; i < nfree; ++i ) addr[i] = S.acquire();
			}
			
			c_shuffle(addr, num_blocks);
			for( size_t i=0; i < num_blocks; ++i )
			{
				S.release( addr[i] );
			}
			
			kind<global>::release( entry, bytes );
		}
		//std::cerr << std::endl;
	}
}
YOCTO_UNIT_TEST_DONE()
