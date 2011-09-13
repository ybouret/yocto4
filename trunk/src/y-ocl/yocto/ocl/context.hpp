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
			explicit Context( cl_uint num_devices, cl_device_id *devices );
			virtual ~Context() throw();
			
			const cl_uint                          NUM_DEVICES;
		private: ArrayOf<cl_device_id>::Type       devices_;
		public:  const cl_device_id               *DEVICES;
			
			virtual void Notify( const char *errinfo, const memory::ro_buffer &errdata) const throw();
			
		private:			
			YOCTO_DISABLE_COPY_AND_ASSIGN(Context);
		};
		
	}
	
}

#endif
