#ifndef YOCTO_OCL_CONTEXT_INCLUDED
#define YOCTO_OCL_CONTEXT_INCLUDED 1

#include "yocto/ocl/device.hpp"

namespace yocto
{
	
	namespace ocl
	{
		//! cl_context wrapper
		class Context : public Shared<cl_context>
		{
		public:
			explicit Context( const DeviceIDs &device_list );
			virtual ~Context() throw();
			
			const cl_uint               NUM_DEVICES;
			const DeviceIDs             devices;
			const cl_device_id         *DEVICES;
			
			virtual void Notify( const char *errinfo, const memory::ro_buffer &errdata) const throw();
			
		private:			
			YOCTO_DISABLE_COPY_AND_ASSIGN(Context);
		};
		
	}
	
}

#endif
