#include "yocto/utest/run.hpp"
#include "yocto/ocl/command-queue.hpp"
#include "yocto/ocl/driver.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/code/rand.hpp"

#include "yocto/ios/ocstream.hpp"

using namespace yocto;

static const char kernel_code[] =
"__kernel void add( __global float c[], __global const float a[], __global const float b[] ) {\n"
" const size_t i = get_global_id(0);\n"
" c[i] = b[i] + a[i];\n"
"}\n";

static inline void display( const char *info, cl_event event )
{
	std::cerr << "Profiling " << info << std::endl;
	const ocl::Event::Profiling pf( event );
	std::cerr << "\t" << info << ".lag = " << pf.LagNanoSeconds() << std::endl;
	std::cerr << "\t" << info << ".run = " << pf.RunNanoSeconds() << std::endl;
	
}

YOCTO_UNIT_TEST_IMPL(profile)
{
	
	YOCTO_OpenCL;
	const ocl::Platform &platform = OpenCL.Platforms[0];
	ocl::Context         context( platform.devices );
	ocl::Sources         sources;
	
	sources << ocl::LoadData  << kernel_code;
	
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
	
	const size_t N = 16384;
	ocl::Kernel kernel( program, "add" );
	ocl::BufferOf<cl_float> ocl_A( context, CL_MEM_READ_WRITE, N, NULL );
	ocl::BufferOf<cl_float> ocl_B( context, CL_MEM_READ_WRITE, N, NULL );
	ocl::BufferOf<cl_float> ocl_C( context, CL_MEM_READ_WRITE, N, NULL );
	{
		cl_uint arg_index = 0;
		kernel.SetArg( arg_index++, ocl_A );
		kernel.SetArg( arg_index++, ocl_B );
		kernel.SetArg( arg_index++, ocl_C );
	}
	vector<cl_float> vec_A(N,0), vec_B(N,0), vec_C(N,0);
	
	
	for( size_t idev = 0; idev < program.NUM_DEVICES; ++idev )
	{
		cl_device_id       device = program.DEVICES[idev];
		const ocl::Device &D      = OpenCL[device];
		std::cerr << std::endl;
		std::cerr << "Executing on " << D.NAME << std::endl;
		
		//if( D.TYPE != CL_DEVICE_TYPE_CPU ) continue;
		
		
		for( size_t i=1; i <= N; ++i )
		{
			vec_B[i] = 0.5f - alea<cl_float>();
			vec_C[i] = 0.5f - alea<cl_float>();
		}
		const size_t global_work_size[] = { N };
		
		try
		{
			ocl::CommandQueue Q( context, device, CL_QUEUE_PROFILING_ENABLE );
			if( Q.can_profile() )
			{
				std::cerr << "Q can profile !" << std::endl;
			}
			else {
				std::cerr << "Q CAN'T profile !" << std::endl;
			}

			ocl::Event::List events;
			cl_event         event_writeB = NULL;
			cl_event         event_writeC = NULL;
			cl_event         event_kernel = NULL;
			cl_event         event_readA  = NULL;
			
			Q.EnqueueWriteBuffer(ocl_B, CL_FALSE, 0, ocl_B.SIZE, vec_B(),   YOCTO_OPENCL_NO_LIST, &event_writeB);    events.insert( event_writeB );
			Q.EnqueueWriteBuffer(ocl_C, CL_FALSE, 0, ocl_C.SIZE, vec_C(),   YOCTO_OPENCL_NO_LIST, &event_writeC);    events.insert( event_writeC );
			Q.EnqueueNDRangeKernel(kernel, 1, NULL, global_work_size, NULL, YOCTO_OPENCL_NO_LIST, &event_kernel);    events.insert( event_kernel );
			Q.EnqueueReadBuffer(ocl_A, CL_FALSE, 0, ocl_A.SIZE, vec_A(),    YOCTO_OPENCL_NO_LIST, &event_readA );    events.insert( event_readA  );
		
			
			events.WaitFor();
			Q.Finish();
					
			
			display( "writeB", event_writeB );
			display( "writeC", event_writeC );
			display( "kernel", event_kernel );
			display( "readA",  event_readA );
			
		}
		catch( const exception &e )
		{
			std::cerr << e.when() << std::endl;
			std::cerr << e.what() << std::endl;
		}
		catch(...)
		{
			std::cerr << "unhandled exception!!" << std::endl;
		}
		
	}
	
	
	
}
YOCTO_UNIT_TEST_DONE()
