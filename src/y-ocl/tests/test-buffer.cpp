#include "yocto/utest/run.hpp"
#include "yocto/ocl/buffer.hpp"
#include "yocto/ocl/driver.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(buffer)
{
	
	YOCTO_OPENCL;
	const ocl::Platform &P = OpenCL.platforms[0];
	ocl::DeviceIDs       devices( P.deviceIDs );
	ocl::Context         context( devices.size, &devices[0] );
	ocl::Buffer          buf1( context, CL_MEM_READ_WRITE, 256, NULL );
	std::cerr << "buf1.SIZE=" << buf1.SIZE << std::endl;

}
YOCTO_UNIT_TEST_DONE()
