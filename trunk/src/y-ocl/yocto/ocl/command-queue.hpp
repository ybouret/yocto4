#ifndef YOCTO_OPENCL_COMMAND_QUEUE_INCLUDED
#define YOCTO_OPENCL_COMMAND_QUEUE_INCLUDED 1

#include "yocto/ocl/buffer.hpp"
#include "yocto/ocl/event.hpp"
#include "yocto/ocl/kernel.hpp"

namespace yocto
{
	
	
	namespace ocl
	{
		
		namespace Core
		{
			
			class CommandQueue : public Object
			{
			public:
				const cl_context                  CONTEXT;
				const cl_device_id                DEVICE;
				const cl_command_queue_properties PROPERTIES;
				
				virtual ~CommandQueue() throw();
				explicit CommandQueue(const ocl::Context &      __context,
									  const cl_device_id          device, 
									  cl_command_queue_properties properties );
				
				bool can_profile() const throw();
				bool is_ooo() const throw();
				
				typedef Proxy<CommandQueue> Pointer;
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN( CommandQueue );
				
			};
			
		}
		
#define YOCTO_OPENCL_NO_EVENT_LIST 0,NULL
#define YOCTO_OPENCL_NO_EVENTS     YOCTO_OPENCL_NO_EVENT_LIST,NULL
		
		//! OpenCL Command Queue
		class CommandQueue: public Class
		{
		public:
			explicit CommandQueue( const Context & __context, const cl_device_id device, cl_command_queue_properties properties );
			virtual ~CommandQueue() throw();
			CommandQueue( const CommandQueue &Q ) throw(); //!< Shared Copy
			
			const Core::CommandQueue *operator->() const throw();
			
			void _OptimizeNDRangeKernel(Kernel             &K,
										cl_uint             work_dim,
										const size_t *      global_work_size,
										size_t *            local_work_size ) const;
			
			void Flush();
			void Finish();
			void EnqueueBarrier();
			
			//==================================================================
			//
			// EnqueueWriteBuffer
			//
			//==================================================================
			
			//------------------------------------------------------------------
			//! simple C++ wrapper
			//------------------------------------------------------------------
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
			
			//------------------------------------------------------------------
			//! simple C++ wrapper
			//------------------------------------------------------------------			
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
			
			//------------------------------------------------------------------
			//! simple C++ wrapper
			//------------------------------------------------------------------			
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
			// EnqueueCopyBuffer
			//
			//==================================================================
			
			//------------------------------------------------------------------
			//! simple C++ wrapper
			//------------------------------------------------------------------			
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
			
			//------------------------------------------------------------------
			//! simple C++ wrapper
			//------------------------------------------------------------------			
			void EnqueueTask(Kernel           &kernel,
							 cl_uint           num_events_in_wait_list,
							 const cl_event   *event_wait_list,
							 cl_event         *event);
			
			//==================================================================
			//
			// Special Events API
			//
			//==================================================================
			cl_event EnqueueMarker();
			void     EnqueueWaitForEvents( const Event::List &todo );
			
			
		private:
			Core::Shared<cl_command_queue>    hCommandQueue_;
			const Core::CommandQueue::Pointer pCommandQueue_;
			
			
			YOCTO_DISABLE_ASSIGN( CommandQueue );
		};
		
	}
	
}

#endif
