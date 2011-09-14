#ifndef YOCTO_OCL_COMMAND_QUEUE_INCLUDED
#define YOCTO_OCL_COMMAND_QUEUE_INCLUDED 1

#include "yocto/ocl/context.hpp"

namespace yocto
{
	namespace ocl
	{
		
		class CommandQueue : public Shared<cl_command_queue>
		{
		public:
			explicit CommandQueue(const Context              &ctx, 
								  const cl_device_id          device,
								  cl_command_queue_properties properties);
			virtual ~CommandQueue() throw();
			
			const cl_device_id          DEVICE;
			cl_command_queue_properties PROPERTIES() const;
			bool is_ooo() const;      //!< Out Of Order ?
			bool can_profile() const; //!< Profiling Enabled ?
			
			void Flush();
			void Finish();
			
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(CommandQueue);
		};
		
	}
}

#endif
