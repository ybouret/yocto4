#include "yocto/utest/run.hpp"
#include "yocto/ocl/driver.hpp"

using namespace yocto;

#define SHOWP(FIELD) std::cerr << "\t" #FIELD << " = " << P.FIELD << std::endl

YOCTO_UNIT_TEST_IMPL(platform)
{
	YOCTO_OPENCL;
	
	std::cerr << "OpenCL.num_platforms=" << OpenCL.num_platforms << std::endl;
	for( cl_uint p = 0; p < OpenCL.num_platforms; ++p )
	{
		const ocl::Platform &P = OpenCL.platforms[p];
		SHOWP(PROFILE);
		SHOWP(VERSION);
		SHOWP(NAME);
		SHOWP(VENDOR);
		SHOWP(EXTENSIONS);
	}
}
YOCTO_UNIT_TEST_DONE()
