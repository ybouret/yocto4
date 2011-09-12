#include "yocto/utest/run.hpp"
#include "yocto/ocl/driver.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(types)
{
	YOCTO_OPENCL;
	
	std::cerr << "OpenCL.num_platforms=" << OpenCL.num_platforms << std::endl;
	
}
YOCTO_UNIT_TEST_DONE()
