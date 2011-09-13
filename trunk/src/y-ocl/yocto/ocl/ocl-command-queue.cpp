#include "yocto/ocl/command-queue.hpp"
#include "yocto/ocl/exception.hpp"

namespace yocto
{
	
	namespace ocl
	{
		
		static inline
		size_t _ref_count( cl_command_queue q )
		{
			cl_uint      ans = 0;
			const cl_int err = clGetCommandQueueInfo( q, CL_QUEUE_REFERENCE_COUNT, sizeof(ans), &ans, NULL);
			if( err != CL_SUCCESS )
			{
				throw Exception( err, "clGetCommandQueueInfo( REF_COUNT )" );
			}
			return ans;
		}
		
		YOCTO_OPENCL_SHARED_IMPL(cl_command_queue,clRetainCommandQueue,clReleaseCommandQueue, _ref_count);
		
		CommandQueue:: ~CommandQueue() throw() {}
		
		
		static inline
		cl_command_queue __create_Q( const Context              &ctx, 
									const cl_device_id          device,
									cl_command_queue_properties properties )
		{
			cl_int           err = CL_SUCCESS; 
			cl_command_queue Q   = clCreateCommandQueue( *ctx, device, properties, &err);
			if( CL_SUCCESS != err )
				throw Exception( err, "clCreateCommandQueue" );
			return Q;
		}
																
		
		CommandQueue:: CommandQueue(const Context              &ctx, 
									const cl_device_id          device,
									cl_command_queue_properties properties ) :
		Shared<cl_command_queue>( __create_Q(ctx,device,properties) ),
		DEVICE( device )
		{
		}
		
		cl_command_queue_properties CommandQueue:: PROPERTIES() const
		{
			cl_command_queue_properties pty = 0;
			const cl_int                err = clGetCommandQueueInfo( *(*this), CL_QUEUE_PROPERTIES, sizeof(pty), &pty, NULL);
			if( CL_SUCCESS != err )
				throw Exception( err, "CL_QUEUE_PROPERTIES" );
			return pty;
		}
		
		bool CommandQueue:: is_ooo() const
		{
			return 0 != ( PROPERTIES() & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE );
		}
		
		bool CommandQueue:: can_profile() const
		{
			return 0 != ( PROPERTIES() & CL_QUEUE_PROFILING_ENABLE );
		}
		
	}
}
