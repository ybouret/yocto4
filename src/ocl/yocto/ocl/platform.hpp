#ifndef YOCTO_PLATFORM_INCLUDED
#define YOCTO_PLATFORM_INCLUDED 1

#include "yocto/ocl/device.hpp"

namespace yocto
{
	
	namespace ocl
	{
		
		
		class Platform
		{
		public:
			explicit Platform( cl_platform_id platform);
			~Platform() throw();
			const cl_platform_id                     id;
			const cl_uint                            num_devices;
			const DeviceIDs                          devices;
		private: ArrayOf<Device>::Type               Devices_;
		public: const memory::records_of<Device>     Devices;
			const string                             PROFILE;    //!< CL_PLATFORM_PROFILE
			const string                             VERSION;    //!< CL_PLATFORM_VERSION
			const string                             NAME;       //!< CL_PLATFORM_NAME
			const string                             VENDOR;     //!< CL_PLATFORM_VENDOR
		private: const string                        EXTENSIONS; //!< CL_PLATFORM_EXTENSIONS
		public: const Extensions                     extensions; //!< Parsed EXTENSIONS
			
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(Platform);
			
		};
		
	}
	
}

#endif
