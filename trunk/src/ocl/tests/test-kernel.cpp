#include "yocto/utest/run.hpp"
#include "yocto/ocl/command-queue.hpp"
#include "yocto/ocl/driver.hpp"
#include "yocto/sequence/vector.hpp"

#include "yocto/ios/ocstream.hpp"

using namespace yocto;


static const char ocl_add_prolog[] =
" __kernel void add( __global float a[]) {\n";

static const char ocl_add_code[] =
"	const size_t i = get_global_id(0);\n"
"	a[i] += i;\n"
;

static const char ocl_add_epilog[] =
"}\n";


YOCTO_UNIT_TEST_IMPL(kernel)
{
	
	YOCTO_OpenCL;
	const ocl::Platform &platform = OpenCL.Platforms[0];
	ocl::Context         context( platform.devices );
	ocl::Sources         sources;
	sources << ocl::LoadData  << ocl_add_prolog << ocl_add_code << ocl_add_epilog;
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
	
	sources.release();
	ocl::Binaries binaries;
	program.get_binaries(binaries);
	for( size_t i=1; i <= binaries.size(); ++i )
	{
		const ocl::Code   &bin = binaries[i];
		const ocl::Device &dev = OpenCL[ program.DEVICES[i-1] ];
		ios::ocstream      fp( dev.name + ".bin", false );
		fp.append(bin->data, bin->size);
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
	
	size_t N = 32;
	vector<cl_float>        vec_a( N, 0 );
	ocl::BufferOf<cl_float> ocl_a( context, CL_MEM_READ_WRITE, N, NULL );
	for( size_t i=1; i <= N; ++i ) vec_a[i] = float(i);
	std::cerr << "a=" << vec_a << std::endl;
	
	cl_uint arg_index = 0;
	k1.SetArg(arg_index++, ocl_a);
	
	const size_t global_work_size[] = { N };
	
	for( size_t idev = 0; idev < program.NUM_DEVICES; ++idev )
	{
		cl_device_id       device = program.DEVICES[idev];
		const ocl::Device &D      = OpenCL[device];
		std::cerr << std::endl;
		std::cerr << "Executing on " << D.NAME << std::endl;
		ocl::CommandQueue Q( context, device, 0 );
		std::cerr << "k1.WORK_GROUP_SIZE=" << k1.WORK_GROUP_SIZE( device ) << std::endl;
		std::cerr << "k1.LOCAL_MEM_SIZE =" << k1.LOCAL_MEM_SIZE(  device ) << std::endl;
		size_t local_work_size[] = { 0 };
		Q.OptimizeNDRangeKernel(k1, 1, global_work_size, local_work_size);
		std::cerr << "local_work_size[0]=" << local_work_size[0] << std::endl;
		
		Q.EnqueueWriteBuffer(ocl_a, CL_FALSE, 0, ocl_a.SIZE, vec_a(), YOCTO_OPENCL_NO_EVENT );
		Q.EnqueueNDRangeKernel(k1, 1, NULL, global_work_size, local_work_size,   YOCTO_OPENCL_NO_EVENT);
		Q.EnqueueReadBuffer(ocl_a, CL_FALSE, 0, ocl_a.SIZE, vec_a(),  YOCTO_OPENCL_NO_EVENT);
		Q.Finish();
		std::cerr << "a=" << vec_a << std::endl;
		
	}
	
	
}
YOCTO_UNIT_TEST_DONE()
