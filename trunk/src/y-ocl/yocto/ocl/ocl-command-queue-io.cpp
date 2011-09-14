#include "yocto/ocl/command-queue.hpp"
#include "yocto/ocl/exception.hpp"

namespace yocto
{
	namespace ocl
	{
		
		//======================================================================
		//
		// EnqueueWriteBuffer
		//
		//======================================================================
		void CommandQueue:: EnqueueWriteBuffer(Buffer         &buffer,
											   cl_bool         blocking_write,
											   size_t          offset,
											   size_t          cb,
											   const void     *ptr,
											   cl_uint         num_events_in_wait_list,
											   const cl_event *event_wait_list,
											   cl_event       *event )
		{
			const cl_int err = clEnqueueWriteBuffer(*(*this),
													*buffer,
													blocking_write,
													offset,
													cb,
													ptr,
													num_events_in_wait_list,
													event_wait_list,
													event);
			if( CL_SUCCESS != err )
				throw Exception( err,  "clEnqueueWriteBuffer" );
		}
		
		
		
		//======================================================================
		//
		// EnqueueWriteBuffer
		//
		//======================================================================
		void CommandQueue:: EnqueueReadBuffer(Buffer         &buffer,
											  cl_bool         blocking_read,
											  size_t          offset,
											  size_t          cb,
											  void *          ptr,
											  cl_uint         num_events_in_wait_list,
											  const cl_event *event_wait_list,
											  cl_event *      event)
		{
			const cl_int err = clEnqueueReadBuffer(*(*this),
												   *buffer,
												   blocking_read,
												   offset,
												   cb,
												   ptr,
												   num_events_in_wait_list,
												   event_wait_list,
												   event);
			if( CL_SUCCESS != err )
				throw Exception( err,  "clEnqueueReadBuffer" );
		}
		
		
		//======================================================================
		//
		// EnqueueCopyBuffer
		//
		//======================================================================
		void CommandQueue:: EnqueueCopyBuffer(Buffer         &src_buffer,
											  Buffer         &dst_buffer,
											  size_t          src_offset,
											  size_t          dst_offset,
											  size_t          cb,
											  cl_uint         num_events_in_wait_list,
											  const cl_event *event_wait_list,
											  cl_event *      event )
		{
			const cl_int err = clEnqueueCopyBuffer(*(*this),
												   *src_buffer,
												   *dst_buffer,
												   src_offset,
												   dst_offset,
												   cb,
												   num_events_in_wait_list,
												   event_wait_list,
												   event);
			if( err != CL_SUCCESS )
				throw Exception( err, "clEnqueueCopyBuffer" );
		}
		
		
	}
	
}
