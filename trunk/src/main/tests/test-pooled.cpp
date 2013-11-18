#include "yocto/utest/run.hpp"
#include "yocto/memory/pooled.hpp"

using namespace yocto;
using namespace memory;


YOCTO_UNIT_TEST_IMPL(pooled)
{
	pooled::allocator g;
	size_t num = 1000;
	
	{
		void  *ptr = g.acquire( num );
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
		g.release_as<double>( ptr, num );
		if( ptr != NULL ) 
		{
			std::cerr << "ptr!=NULL" << std::endl;
			return -1;
		}
	}
	
}
YOCTO_UNIT_TEST_DONE()
