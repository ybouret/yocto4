#ifndef YOCTO_OCL_COMMAND_QUEUE_INCLUDED
#define YOCTO_OCL_COMMAND_QUEUE_INCLUDED 1

#include "yocto/ocl/buffer.hpp"
#include "yocto/ocl/event.hpp"
#include "yocto/ocl/kernel.hpp"

namespace yocto
{
	namespace ocl
	{
		
#define YOCTO_OPENCL_NO_LIST   0,NULL
#define YOCTO_OPENCL_NO_EVENT  YOCTO_OPENCL_NO_LIST,NULL
		
		class CommandQueue : public Shared<cl_command_queue>
		{
		public:
			explicit CommandQueue(const Context              &ctx, 
								  const cl_device_id          device,
								  cl_command_queue_properties properties);
			virtual ~CommandQueue() throw();
			
			//==================================================================
			//
			// 
			//
			//==================================================================
			const cl_device_id          DEVICE;
			cl_command_queue_properties PROPERTIES() const;
			
			//==================================================================
			//
			// shortcuts
			//
			//==================================================================
			bool is_ooo() const;      //!< Out Of Order ?
			bool can_profile() const; //!< Profiling Enabled ?
			
			//==================================================================
			//
			// Control API
			//
			//==================================================================
			void Flush();
			void Finish();
			void EnqueueBarrier();
			
			//==================================================================
			//
			// Events API
			//
			//==================================================================
			cl_event EnqueueMarker();
			void     EnqueueWaitForEvents( const Event::List &todo );
			
			//==================================================================
			//
			// EnqueueWriteBuffer
			//
			//==================================================================
			
			void EnqueueWriteBuffer(Buffer         &buffer,
									cl_bool         blocking_write,
									size_t          offset,
									size_t          cb,
									const void *    ptr,
									cl_uint         num_events_in_wait_list,
									const cl_event *event_wait_list,
									cl_event *      event );
			
			//==================================================================
			//
			// EnqueueReadBuffer
			//
			//==================================================================
			
			void EnqueueReadBuffer(Buffer         &buffer,
								   cl_bool         blocking_read,
								   size_t          offset,
								   size_t          cb,
								   void *          ptr,
								   cl_uint         num_events_in_wait_list,
								   const cl_event *event_wait_list,
								   cl_event *      event);
			
			//==================================================================
			//
			// EnqueueCopyBuffer
			//
			//==================================================================
			
			void EnqueueCopyBuffer(Buffer         &src_buffer,
								   Buffer         &dst_buffer,
								   size_t          src_offset,
								   size_t          dst_offset,
								   size_t          cb,
								   cl_uint         num_events_in_wait_list,
								   const cl_event *event_wait_list,
								   cl_event *      event);
			
			//==================================================================
			//
			// EnqueueNDRangeKernel
			//
			//==================================================================
			
			void EnqueueNDRangeKernel(Kernel           &kernel,
									  cl_uint           work_dim,
									  const size_t *    global_work_offset,
									  const size_t *    global_work_size,
									  const size_t *    local_work_size,
									  cl_uint           num_events_in_wait_list,
									  const cl_event  * event_wait_list,
									  cl_event        * event);
			
			//==================================================================
			//
			// EnqueueTask
			//
			//==================================================================
			
			void EnqueueTask(Kernel           &kernel,
							 cl_uint           num_events_in_wait_list,
							 const cl_event   *event_wait_list,
							 cl_event         *event);
			
			
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(CommandQueue);
		};
		
	}
}

#endif
