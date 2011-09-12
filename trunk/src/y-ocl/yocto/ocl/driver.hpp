//! \file driver.hpp
/**
 OpenCL Platform Layer wrapper
 */
#ifndef YOCTO_OPENCL_DRIVER_INCLUDED
#define YOCTO_OPENCL_DRIVER_INCLUDED 1

#include "yocto/ocl/platform.hpp"
#include "yocto/threading/singleton.hpp"
#include "yocto/auto-ptr.hpp"

#define YOCTO_OPENCL_SINGLETON singleton<Driver>
#define YOCTO_OPENCL_LIFE_TIME (7)
#define YOCTO_OpenCL ocl::Driver & OpenCL = ocl::Driver::instance()

namespace yocto 
{
	
	
	namespace ocl
	{
		namespace Core
		{
			typedef ArrayOf<cl_platform_id>::Type     PlatformIDs;    //!< array   of  cl_platform_id
			typedef   ArrayOf<ocl::Platform>::Type    PlatformsArray; //!< memory  for Platforms
			typedef RecordsOf<ocl::Platform>::Type    Platforms;      //!< records of  Platform
			typedef RecordsOf<cl_device_id>::Type     DeviceIDsMap;   //!< selection of devices
			typedef ArrayOf<string>::Type             StringsArray;
			typedef RecordsOf<string>::Type           Strings;
		}
		
		class Program;
		
		class Driver : public YOCTO_OPENCL_SINGLETON
		{
		public:
			const cl_uint           num_platforms; //!< #platforms on this O/S
			const Core::PlatformIDs platform_id;   //!< their id [0..num_platforms-1]
			
		private:
			Core::PlatformsArray    platform_;    //!< memory for platforms
			
		public:
			const Core::Platforms   platform;     //!< where platforms are stored
			const cl_uint           max_devices;  //!< grand total of devices
			
		private:
			mutable Core::DeviceIDsMap device_map;  //!<
			
		public:
			Core::Strings              build_log;
			
			//! dictionary like
			const Device &        operator[]( const cl_device_id  ) const;
			
			void                  Compile(Program &               __program, 
										  cl_uint				  num_devices,
										  const cl_device_id     *device_list,
										  const char             *options);
			void Compile( Program & __program, const char *options ); //!< compile for all devices in associated context
			void BuildLogOut( std::ostream & ) const;
			
			const Core::DeviceIDsMap & MapDeviceIDs( const string &map ) const;
			
			void CreateDeviceMapper() const;
			void DeleteDeviceMapper() const throw();
			
		private:
			class DeviceMapper;
			void *wksp_;
			cl_device_id               *device_id_;   //!< for max_devices
			string                     *build_log_;   //!< for max_devices
			mutable DeviceMapper       *mapper_;
			
			
			explicit Driver();
			virtual ~Driver() throw();
			
			
			
			YOCTO_DISABLE_COPY_AND_ASSIGN(Driver);
			static const threading::longevity life_time = YOCTO_OPENCL_LIFE_TIME;
			friend class YOCTO_OPENCL_SINGLETON;
		};
	}
	
}

#endif
