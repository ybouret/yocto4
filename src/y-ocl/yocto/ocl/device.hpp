#ifndef YOCTO_OCL_DEVICE_INCLUDED
#define YOCTO_OCL_DEVICE_INCLUDED 1

#include "yocto/ocl/types.hpp"

namespace yocto
{
	namespace ocl
	{
		
		class Device 
		{
		public:
			cl_device_id id;
			
			explicit Device( cl_device_id device );
			~Device() throw();
			
			bool is_available() const;
			
			const cl_bool                     COMPILER_AVAILABLE;
			const cl_bool                     ERROR_CORRECTION_SUPPORT;
		private: const string                 EXTENSIONS;
		public: const Extensions              extensions;
			const cl_ulong                    GLOBAL_MEM_CACHE_SIZE;
			const cl_ulong                    GLOBAL_MEM_SIZE;
			const bool                        IMAGE_SUPPORT;
			const cl_ulong                    LOCAL_MEM_SIZE;
			const cl_uint                     MAX_CLOCK_FREQUENCY;
			const cl_uint                     MAX_COMPUTE_UNITS;
			const cl_ulong                    MAX_CONSTANT_BUFFER_SIZE;
			const cl_ulong                    MAX_MEM_ALLOC_SIZE;
			const size_t                      MAX_WORK_GROUP_SIZE;
			const cl_uint                     MAX_WORK_ITEM_DIMENSIONS;
			const ArrayOf<size_t>::Type       MAX_WORK_ITEM_SIZES;
			const string                      NAME;
			const cl_platform_id              PLATFORM;
			const string                      PROFILE;
			const size_t                      PROFILING_TIMER_RESOLUTION;
			const cl_command_queue_properties QUEUE_PROPERTIES;
			const cl_device_type              TYPE;
			const string                      VENDOR;
			const string                      VENDOR_ID;
			const string                      VERSION;
			const string                      DRIVER_VERSION;
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(Device);
		};
		

		
	}
	
}

#endif
