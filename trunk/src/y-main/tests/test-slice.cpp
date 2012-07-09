#include "yocto/utest/run.hpp"
#include "yocto/memory/slice.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/memory/global.hpp"

using namespace yocto;
using namespace memory;

YOCTO_UNIT_TEST_IMPL(slice)
{
	std::cerr << "sizeof(slice)=" << sizeof(slice) << std::endl;
	std::cerr << "sizeof(block_t)=" << sizeof(slice::block_t) << std::endl;
	{
		std::cerr << "block alignment" << std::endl;
		size_t s = 0;
		for( size_t i=0; i <10; ++i )
		{
			for( size_t j=0; j < 8; ++j, s += (i+j) )
			{
				std::cerr << " (" << s << ":" << slice::blocks_for(s)*sizeof(slice::block_t) << ")";
			}
			std::cerr << std::endl;
		}
	}
	
	for( size_t k = 0; k < 16; ++k )
	{
		const size_t mini_size = 100 + alea_leq(4000);
		std::cerr << "#blocks for " << mini_size << " = " << slice::blocks_for( mini_size ) << std::endl;
		slice S(0,mini_size);
		std::cerr << "S.count=" << S.count() << std::endl;
		std::cerr << "S.bytes=" << S.bytes() << std::endl;
		
		//-- full length acquire
		size_t length = mini_size;
		void *p = S.acquire(length);
		if( !p )
		{
			throw exception("unexpected slice.acquire failure!");
		}
		std::cerr << "acquired length=" << length << " | " << length/sizeof(slice::block_t) <<  " blk" << std::endl;
		S.release(p);
		
		size_t nmax = S.count();
		void **addr = kind<global>::acquire_as<void *>( nmax );
		int    num  = 0;
		for(;;)
		{
			std::cerr << "+";
			length = 1 + alea_lt(8*sizeof(slice::block_t));
			//std::cerr << "? " << length << " => "; 
			p      = S.acquire(length);
			if( !p )
			{
				//std::cerr << "out of mem" << std::endl;
				break;
			}
			//std::cerr <<  length << " | " << length/sizeof(slice::block_t) <<  " blk" << std::endl;
			addr[num++] = p;
		}
		
		for( size_t iter = 0; iter < 16; ++iter )
		{
			std::cerr << "-";
			c_shuffle(addr, num);
			const int ntop = num/2;
			while (num>ntop)
			{
				S.release( addr[--num] );
			}
			
			std::cerr << "+";
			for(;;)
			{
				length = 1 + alea_lt(8*sizeof(slice::block_t));
				//std::cerr << "? " << length << " => "; 
				p      = S.acquire(length);
				if( !p )
				{
					//std::cerr << "out of mem" << std::endl;
					break;
				}
				//std::cerr <<  length << " | " << length/sizeof(slice::block_t) <<  " blk" << std::endl;
				addr[num++] = p;			
			}
		}
		
		std::cerr << "-";
		c_shuffle(addr, num);
		for( int i=0; i < num; ++i )
		{
			S.release( addr[i] );
		}
		
		kind<global>::release_as<void *>( addr, nmax );
		std::cerr << std::endl;
		
	}
}
YOCTO_UNIT_TEST_DONE()

