#define _CRT_SECURE_NO_WARNINGS 1
#include "yocto/utest/run.hpp"
#include "yocto/memory/arena.hpp"
#include "yocto/memory/chunk.hpp"
#include "yocto/code/round.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/sys/wtime.hpp"
#include "yocto/memory/global.hpp"

#include <cstdlib>
#include <cstdio>

using namespace yocto;
using namespace memory;


YOCTO_UNIT_TEST_IMPL(arena)
{
	size_t  num   = 0;
	size_t  niter = 0;
	
	if( argc > 1 )
		num = strtol( argv[1], NULL, 10);
	
	if( argc > 2 )
		niter = strtol( argv[2], NULL, 10);
	
	if( num <= 0 )
		num = 300;
	
	if( niter <= 0 )
		niter = 1;
	
	void **addr = kind<global>::acquire_as<void*>(num);
	
	wtime chrono;
	FILE *fp = fopen( "bs.dat", "wb" );
	std::cerr << "sizeof(arena)=" << sizeof(arena) << std::endl;
	try
	{
		for( size_t block_size=1; block_size <= 20; block_size += ( 1 + alea_leq(4) ) )
		{
			std::cerr << "block_size =" << block_size << std::endl;
			for( size_t num_blocks = chunk::num_blocks_min; 
				num_blocks        <= chunk::num_blocks_max; 
				num_blocks += ( 1 + alea_leq(4) ))
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
				
				for( size_t iter=0; iter<niter; ++iter)
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
				if( fp )
				{
					fprintf( fp, "%u %u %g\n", unsigned(block_size), unsigned(num_blocks), tmx/num );
				}
			}
		}
		if(fp) fclose( fp );
	}
	catch(...)
	{
		if( fp ) fclose(fp);
		throw;
	}
	
	kind<global>::release_as<void*>( addr, num );
	
}
YOCTO_UNIT_TEST_DONE()
