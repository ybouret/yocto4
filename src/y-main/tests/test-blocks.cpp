#include "yocto/memory/blocks.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/code/rand.hpp"


#include <cstring>
#include <cstdlib>

using namespace yocto;
using namespace memory;

namespace  
{
	struct block_t
	{
		size_t size;
		void  *addr;
	};
}

YOCTO_UNIT_TEST_IMPL(blocks)
{
	
	size_t num = 0;
	
	if( argc > 1 )
		num = strtol( argv[1], NULL, 10);
	
	if( num <= 0 )
		num = 512;
	
	block_t *b = kind<global>::acquire_as<block_t>( num );
	try
	{
		for( size_t iter =0; iter < 16 ; ++iter )
		{
			const size_t chunk_size = 100 + alea_less_than(1000);
			std::cerr << "blocks with chunk_size=" << chunk_size << std::endl;
			blocks blk( chunk_size );
			for( size_t i=0; i < num; ++i )
			{
				b[i].size = 1 + alea_less_than(50);
				b[i].addr = blk.acquire( b[i].size );
			}
			
			const size_t nfree = num / 2;
			
			for( size_t scramble=0; scramble < 16; ++scramble)
			{
				c_shuffle(b, num);
				for( size_t i=nfree; i < num; ++i )
				{
					blk.release( b[i].addr, b[i].size);
				}
				
				for( size_t i=nfree; i < num; ++i )
				{
					b[i].size = 1 + alea_less_than(50);
					b[i].addr = blk.acquire( b[i].size );
				}
			}
			
			
			c_shuffle(b, num);
			for( size_t i=0; i < num; ++i )
			{
				blk.release( b[i].addr, b[i].size);
			}
			
		}
	}
	catch(...)
	{
		kind<global>:: release_as<block_t>( b, num );
		throw;
	}
	kind<global>:: release_as<block_t>( b, num );
	
	
	
	
	
}
YOCTO_UNIT_TEST_DONE()


#include "yocto/object.hpp"

YOCTO_UNIT_TEST_IMPL(small_object)
{
	size_t num = 0;
	
	if( argc > 1 )
		num = strtol( argv[1], NULL, 10);
	
	if( num <= 0 )
		num = 512;
	
	block_t *b = kind<global>::acquire_as<block_t>( num );
	try
	{
		
		for( size_t i=0; i < num; ++i )
		{
			b[i].size = 1 + alea_less_than( 2 * object::limit_size );
			b[i].addr = object::operator new( b[i].size );
		}
		
		const size_t nfree = num / 2;
		
		for( size_t scramble=0; scramble < 16; ++scramble)
		{
			c_shuffle(b, num);
			for( size_t i=nfree; i < num; ++i )
			{
				object::operator delete( b[i].addr, b[i].size);
			}
			
			for( size_t i=nfree; i < num; ++i )
			{
				b[i].size = 1 + alea_less_than( 2 * object::limit_size );
				b[i].addr = object::operator new( b[i].size );
			}
		}
		
		
		c_shuffle(b, num);
		for( size_t i=0; i < num; ++i )
		{
			object::operator delete( b[i].addr, b[i].size);
		}
		
		
	}
	catch(...)
	{
		kind<global>::release_as<block_t>(b,num);
		throw;
	}
	
	kind<global>::release_as<block_t>(b,num);
	
}
YOCTO_UNIT_TEST_DONE()

