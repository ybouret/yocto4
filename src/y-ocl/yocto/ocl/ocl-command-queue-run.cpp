#include "yocto/ocl/command-queue.hpp"
#include "yocto/ocl/exception.hpp"

namespace yocto
{
	namespace ocl
	{
		
		//======================================================================
		//
		// EnqueueTask
		//
		//======================================================================
		void CommandQueue:: EnqueueTask(Kernel          &kernel,
										cl_uint           num_events_in_wait_list,
										const cl_event   *event_wait_list,
										cl_event         *event)
		{
			const cl_int err = clEnqueueTask(*(*this),
											 *kernel, 
											 num_events_in_wait_list,
											 event_wait_list, 
											 event);
			if( err != CL_SUCCESS )
				throw Exception( err, "clEnqueueTask" );
		}
		
		
		//======================================================================
		//
		// EnqueueNDRangeKernel
		//
		//======================================================================
		void CommandQueue:: EnqueueNDRangeKernel(Kernel           &kernel,
												 cl_uint           work_dim,
												 const size_t *    global_work_offset,
												 const size_t *    global_work_size,
												 const size_t *    local_work_size,
												 cl_uint           num_events_in_wait_list,
												 const cl_event  * event_wait_list,
												 cl_event        * event)
		{
			const cl_int err = clEnqueueNDRangeKernel(*(*this),
													  *kernel,
													  work_dim,
													  global_work_offset,
													  global_work_size,
													  local_work_size,
													  num_events_in_wait_list,
													  event_wait_list,
													  event);
			if( CL_SUCCESS != err )
				throw Exception( err, "clEnqueueNDRangeKernel" );
		}
		
	}
	
}

