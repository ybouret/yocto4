#include "yocto/utest/run.hpp"
#include "yocto/ocl/buffer.hpp"
#include "yocto/ocl/driver.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(buffer)
{
	
	YOCTO_OPENCL;
	const ocl::Platform &P = OpenCL.Platforms[0];
	ocl::DeviceIDs       devices( P.devices );
	ocl::Context         context( devices );
	ocl::Buffer          buf1( context, CL_MEM_READ_WRITE, 256, NULL );
	std::cerr << "buf1.SIZE=" << buf1.SIZE << std::endl;

}
YOCTO_UNIT_TEST_DONE()
