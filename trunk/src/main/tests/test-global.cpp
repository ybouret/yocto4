#include "yocto/utest/run.hpp"
#include "yocto/memory/global.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(global)
{
	memory::global::allocator g;
	size_t num = 1000;
	
	{
		void  *ptr = g.acquire( num );
		std::cerr << "allocated=" << memory::global::allocated() << std::endl;
		g.release( ptr, num );
		if( ptr != NULL ) 
		{
			std::cerr << "ptr!=NULL" << std::endl;
			return -1;
		}
	}
	
	{
		num = 1000;
		double *ptr = g.acquire_as<double>(num);
		std::cerr << "allocated=" << memory::global::allocated() << std::endl;
		g.release_as<double>( ptr, num );
		if( ptr != NULL ) 
		{
			std::cerr << "ptr!=NULL" << std::endl;
			return -1;
		}
	}
	
	std::cerr << "allocated=" << memory::global::allocated() << std::endl;
	
}
YOCTO_UNIT_TEST_DONE()

