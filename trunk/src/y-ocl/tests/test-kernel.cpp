#include "yocto/utest/run.hpp"
#include "yocto/ocl/kernel.hpp"
#include "yocto/ocl/driver.hpp"

using namespace yocto;


static const char ocl_add[] =
" __kernel void add( __global float a[]) {} ";

YOCTO_UNIT_TEST_IMPL(kernel)
{

	YOCTO_OPENCL;
	const ocl::Platform &platform = OpenCL.Platforms[0];
	ocl::Context         context( platform.devices );
	ocl::Sources         sources;
	sources << ocl::LoadData << ocl_add;
	ocl::Program         program( context, sources );
	
	try {
		OpenCL.Build( program, context.devices, NULL );
		std::cerr << "SUCCESS!" << std::endl;
		OpenCL.BuildLogsOut( std::cerr );
	}
	catch (...) {
		std::cerr << "FAILURE!" << std::endl;
		OpenCL.BuildLogsOut( std::cerr );
		throw;
	}
	
	std::cerr << "Compiled for ";
	for( size_t i=0; i < context.NUM_DEVICES; ++i ) std::cerr << "<" << OpenCL[ context.DEVICES[i] ].NAME << "> ";
	std::cerr << std::endl;
	std::cerr << "Results in   ";
	for( size_t i=0; i < program.NUM_DEVICES; ++i ) std::cerr << "<" << OpenCL[ program.DEVICES[i] ].NAME << "> ";
	std::cerr << std::endl;
	
	ocl::Kernel k1( program, "add" );
	std::cerr << "k1.FUNCTION_NAME=" << k1.FUNCTION_NAME << std::endl;
	std::cerr << "k1.NUM_ARGS     =" << k1.NUM_ARGS      << std::endl;
	
	
}
YOCTO_UNIT_TEST_DONE()
