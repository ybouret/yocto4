#ifndef YOCTO_OCL_DRIVER_INCLUDED
#define YOCTO_OCL_DRIVER_INCLUDED 1


#include "yocto/ocl/types.hpp"
#include "yocto/threading/singleton.hpp"


namespace yocto
{
	namespace  ocl 
	{
		typedef Core::ArrayOf<cl_platform_id>::Type PlatformIDs;
		
		class Driver : public singleton<Driver>
		{
		public:
			const cl_uint       num_platforms;
			const PlatformIDs   platforms;
			
		private:
			explicit Driver();
			virtual ~Driver() throw();
			static const threading::longevity life_time = 7;
			static const char                 name[];
			YOCTO_DISABLE_COPY_AND_ASSIGN(Driver);
			friend class singleton<Driver>;
		};
	
#define YOCTO_OPENCL ocl::Driver & OpenCL = ocl::Driver::instance()
		
	}
}


#endif
