#include "yocto/utest/run.hpp"
#include "yocto/ocl/context.hpp"
#include "yocto/ocl/driver.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(context)
{
	std::cerr << "-- Loading OpenCL" << std::endl;
	YOCTO_OpenCL;
	const ocl::Platform &platform  = OpenCL.Platforms[0];
	std::cerr << "-- Using platform '" <<  platform.NAME << "'" << std::endl;

	for( size_t nd = 1; nd <= platform.num_devices; ++nd )
	{
		ocl::DeviceIDs devs( nd );
		for( size_t i=0; i < nd; ++i ) 
			devs[i] = platform.devices[i];
	
		c_shuffle( devs(), devs.size );
		
		std::cerr << "-- creating context from ";
		for( size_t i=0; i < nd; ++i ) std::cerr << "<" << OpenCL[ devs[i] ].NAME << "> ";
		std::cerr << std::endl;
		ocl::Context context( devs );
		std::cerr << "-- context context :     ";
		for( size_t i=0; i < context.NUM_DEVICES; ++i ) std::cerr << "<" << OpenCL[ context.DEVICES[i] ].NAME << "> ";
		std::cerr << std::endl;
		std::cerr << std::endl;
	}
}
YOCTO_UNIT_TEST_DONE()

