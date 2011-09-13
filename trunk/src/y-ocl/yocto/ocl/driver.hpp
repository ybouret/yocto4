#ifndef YOCTO_OCL_DRIVER_INCLUDED
#define YOCTO_OCL_DRIVER_INCLUDED 1


#include "yocto/ocl/platform.hpp"
#include "yocto/threading/singleton.hpp"


namespace yocto
{
	namespace  ocl 
	{
		
		class Driver : public singleton<Driver>
		{
		public:
			const cl_uint                               num_platforms;
			const ArrayOf<cl_platform_id>::Type         platformIDs;
		private: ArrayOf<Platform>::Type                platforms_;
		public: const memory::records_of<Platform>      platforms;
			const cl_uint                               num_devices;
			
			const Device & operator[]( const cl_device_id ) const;
			
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
