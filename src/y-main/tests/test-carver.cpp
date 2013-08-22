#include "yocto/utest/run.hpp"
#include "yocto/memory/carver.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/sort/gnome.hpp"

#include <cstdlib>

using namespace yocto;
using namespace memory;

YOCTO_UNIT_TEST_IMPL(carver)
{
	carver C(1000,32);
	
	size_t num = 0;
	if( argc > 1 )
	{
		num = strtol(argv[1], NULL, 10);
	}
	
	if( num <= 0 )
		num = 1000;
	
	std::cerr << "sizeof(carver)=" << sizeof(carver) << std::endl;
	
	void **addr = kind<global>::acquire_as<void*>(num);
	
	try 
	{
		for( size_t i=0; i < num; ++i )
		{
			size_t length = 1 + alea_leq(100);
			addr[i] = C.acquire( length );
		}
		std::cerr << "#slices=" << C.used_slices() << std::endl;
		
		const size_t nfree = (3*num)/4;
		for( size_t j=0; j < 16; ++j )
		{
			c_shuffle(addr, num);
			for( size_t i=0; i < nfree; ++i )
			{
				C.release(addr[i]);
			}
			std::cerr << "#slices=" << C.used_slices() << std::endl;
			for( size_t i=0; i < nfree; ++i )
			{
				size_t length = 1 + alea_leq(100);
				addr[i] = C.acquire( length );
			}
			
		}
		
		c_sort(addr, num);
		for( size_t i=0; i < nfree; ++i )
		{
			C.release( addr[i] );
		}
		std::cerr << "#slices  =" << C.used_slices() << std::endl;
		std::cerr << "#in pool =" << C.pool_slices() << std::endl;
		for( size_t i=nfree; i < num; ++i )
		{
			C.release( addr[i] );
		}
		std::cerr << "#slices  =" << C.used_slices() << std::endl;
		std::cerr << "#in pool =" << C.pool_slices() << std::endl;
	}
	catch(...) {
		kind<global>::release_as<void*>(addr,num);
		throw;
	}
	
	kind<global>::release_as<void*>(addr,num);
	
		
	
}
YOCTO_UNIT_TEST_DONE()
