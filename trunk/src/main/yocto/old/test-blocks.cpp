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
			const size_t chunk_size = 100 + alea_leq(1000);
			std::cerr << "blocks with chunk_size=" << chunk_size << std::endl;
			blocks blk( chunk_size );
			for( size_t i=0; i < num; ++i )
			{
				b[i].size = 1 + alea_leq(50);
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
					b[i].size = 1 + alea_leq(50);
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

#include "yocto/core/list.hpp"

namespace
{
    template <size_t N>
    struct node_type
    {
        node_type *prev;
        node_type *next;
        char       data[N];
    };

    template <size_t N>
    static inline
    void test_blocks_perf(size_t num)
    {
        
        typedef node_type<N>  node_t;
        core::list_of<node_t> nodes;
        std::cerr << "N=" << N << " / sizeof(node_type)=" << sizeof(node_type<N>) << std::endl;
        
        blocks blk(4096);
        for(size_t i=0;i<num;++i)
        {
            node_t *node = (node_t *)(blk.acquire(sizeof(node_t)));
            nodes.push_back(node);
        }
        
        while(nodes.size) blk.release(nodes.pop_front(), sizeof(node_t));
        
    }
    
}

#include "yocto/sys/wtime.hpp"
#include "yocto/ios/ocstream.hpp"

YOCTO_UNIT_TEST_IMPL(blocks_perf)
{
    
    wtime chrono;
    chrono.start();
    
    ios::ocstream::overwrite("blocks_perf.dat");
    
    for(size_t num=1024;num<=100000;num <<= 1)
    {
        double tmx = chrono.query();
        test_blocks_perf<8>(num);
        const double ell8 = chrono.query() - tmx;
        
        tmx = chrono.query();
        test_blocks_perf<16>(num);
        const double ell16 = chrono.query() - tmx;
        
        tmx = chrono.query();
        test_blocks_perf<24>(num);
        const double ell24 = chrono.query() - tmx;
        
        
        ios::ocstream fp("blocks_perf.dat",true);
        fp("%u %g %g %g\n",(unsigned)num,ell8,ell16,ell24);
    }
    
    
    
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
			b[i].size = 1 + alea_leq( 2 * object::limit_size );
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
				b[i].size = 1 + alea_leq( 2 * object::limit_size );
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

