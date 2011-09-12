#include "yocto/utest/run.hpp"
#include "yocto/ocl/types.hpp"
#include "yocto/ocl/exception.hpp"

#include "yocto/memory/pooled.hpp"
#include "yocto/memory/buffers.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(types)
{
	cl_uint num_platforms = 0;
	cl_int err = clGetPlatformIDs(0, NULL, &num_platforms);
	if( err != CL_SUCCESS )
		throw ocl::Exception( err, "clGetPlatformIDs level-1" );
	std::cerr << "num_platforms=" << num_platforms << std::endl;

	memory::buffer_of<cl_platform_id,memory::pooled> platforms( num_platforms );
	
	err = clGetPlatformIDs(num_platforms, &platforms[0], NULL);
	if( err != CL_SUCCESS )
		throw ocl::Exception( err, "clGetPlatformIDs level-2" );
	
}
YOCTO_UNIT_TEST_DONE()
