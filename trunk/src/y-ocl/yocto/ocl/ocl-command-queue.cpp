#include "yocto/ocl/command-queue.hpp"
#include "yocto/ocl/_query.hpp"

namespace yocto
{
	
	
	namespace ocl
	{
		namespace Core
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
			
			CommandQueue:: ~ CommandQueue() throw()
			{
				
			}
			
			CommandQueue:: CommandQueue(const ocl::Context &           __context, 
										const cl_device_id               device,
										cl_command_queue_properties      properties ):
			CONTEXT( * __context    ),
			DEVICE(      device     ),
			PROPERTIES(  properties )
			{
				
			}
			
			bool CommandQueue:: can_profile() const throw()
			{
				return ( PROPERTIES & CL_QUEUE_PROFILING_ENABLE ) != 0;
			}
			
			bool CommandQueue:: is_ooo() const throw()
			{
				return (PROPERTIES & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE) != 0;
			}
		}
		
		CommandQueue:: ~CommandQueue() throw()
		{
		}
		
		static inline
		cl_command_queue __create_Q( cl_context context, cl_device_id device, cl_command_queue_properties &properties )
		{
			//-- phase 1: create the queue
			cl_int           err = CL_SUCCESS;
			cl_command_queue Q   = clCreateCommandQueue( context, device, properties, &err);
			if( CL_SUCCESS != err )
				throw Exception( err, "clCreateCommandQueue" );
			
			//-- phase 2: check properties
			try
			{
				properties = Core::GetInfoValue(type2type<cl_command_queue_properties>(),
												clGetCommandQueueInfo,
												Q, 
												CL_QUEUE_PROPERTIES,
												"CL_QUEUE_PROPERTIES"
												);
			}
			catch(...)
			{
				clReleaseCommandQueue(Q);
				throw;
			}
			return Q;
			
		}
		
		CommandQueue:: CommandQueue( const Context & __context, cl_device_id device, cl_command_queue_properties properties ) :
		hCommandQueue_( __create_Q( * __context,  device, properties)       ),
		pCommandQueue_( new Core::CommandQueue( __context, device, properties ) )
		{
			
		}
		
		CommandQueue:: CommandQueue( const CommandQueue &Q ) throw() :
		hCommandQueue_( Q.hCommandQueue_ ),
		pCommandQueue_( Q.pCommandQueue_ )
		{}
		
		const Core::CommandQueue * CommandQueue:: operator->() const throw()
		{
			return &( *pCommandQueue_ );
		}
		
		void CommandQueue:: Finish()
		{
			const cl_int err = clFinish( *hCommandQueue_ );
			if( CL_SUCCESS != err )
			{
				throw Exception( err, "clFinish" );
			}
		}
		
		void CommandQueue:: Flush()
		{
			const cl_int err = clFlush( *hCommandQueue_ );
			if( CL_SUCCESS != err )
			{
				throw Exception( err, "clFlush" );
			}
		}
		
		void CommandQueue:: EnqueueBarrier()
		{
			const cl_int err = clEnqueueBarrier( *hCommandQueue_ );
			if( CL_SUCCESS != err )
			{
				throw Exception( err, "clEnqueueBarrier" );
			}
		}
		
		
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
			const cl_int err = clEnqueueWriteBuffer( *hCommandQueue_,
													buffer->ASSOCIATED_MEMOBJECT,
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
			const cl_int err = clEnqueueReadBuffer( *hCommandQueue_,
												   buffer->ASSOCIATED_MEMOBJECT,
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
			const cl_int err = clEnqueueCopyBuffer( *hCommandQueue_,
												   src_buffer->ASSOCIATED_MEMOBJECT,
												   dst_buffer->ASSOCIATED_MEMOBJECT,
												   src_offset,
												   dst_offset,
												   cb,
												   num_events_in_wait_list,
												   event_wait_list,
												   event);
			if( err != CL_SUCCESS )
				throw Exception( err, "clEnqueueCopyBuffer" );
		}
		
		
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
			const cl_int err = clEnqueueTask( *hCommandQueue_, * kernel.hKernel_, num_events_in_wait_list, event_wait_list, event );
			if( err != CL_SUCCESS )
				throw Exception( err, "clEnqueueTask" );
		}
		
		
		void CommandQueue:: EnqueueNDRangeKernel(Kernel           &kernel,
												 cl_uint           work_dim,
												 const size_t *    global_work_offset,
												 const size_t *    global_work_size,
												 const size_t *    local_work_size,
												 cl_uint           num_events_in_wait_list,
												 const cl_event  * event_wait_list,
												 cl_event        * event)
		{
			const cl_int err = clEnqueueNDRangeKernel(*hCommandQueue_,
													  *kernel.hKernel_,
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
		
		//======================================================================
		//
		// Events API
		//
		//======================================================================
		cl_event CommandQueue:: EnqueueMarker()
		{
			cl_event     event = NULL;
			const cl_int err   = clEnqueueMarker( *hCommandQueue_, &event);
			if( CL_SUCCESS != err )
				throw Exception( err, "clEnqueueMarker" );
			return event;
		}
		
		void     CommandQueue:: EnqueueWaitForEvents( const Event::List &todo )
		{
			const cl_int err = clEnqueueWaitForEvents( *hCommandQueue_, todo.num_events(), todo() );
			if( CL_SUCCESS != err )
				throw Exception( err, "clEnqueueWaitForEvents");
		}
		
	}
	
}
