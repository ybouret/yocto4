#ifndef YOCTO_OPENCL_PLATFORM_INCLUDED
#define YOCTO_OPENCL_PLATFORM_INCLUDED 1

#include "yocto/ocl/device.hpp"

namespace yocto 
{
	
	namespace ocl
	{
		namespace Core 
		{
			typedef ArrayOf<cl_device_id>::Type  DeviceIDs;
			typedef ArrayOf<ocl::Device>::Type   DevicesArray;
			typedef RecordsOf<ocl::Device>::Type Devices;
			
			//! Platform Infos
			class Platform : public Object
			{
			public:
				explicit Platform( const cl_platform_id);
				virtual ~Platform() throw();
				
				const cl_platform_id id;
				const string         NAME;
				const cl_uint        num_devices;
				const DeviceIDs      device_id;
				
			private:
				DevicesArray         device_;
			public:
				const Devices        device;
				const string         PROFILE;
				const string         VERSION;
				const string         VENDOR;
			private:
				string               EXTENSIONS; //!< will be parsed
			public:
				const Extensions     extension; //!< parsed extensions
				
				typedef Proxy<Platform> Pointer;
				
				cl_device_id  DefaultDeviceID() const;
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(Platform);
				
			};
			
		}
		
		class Driver;
		class Platform
		{
		public:
			
			Platform( const Platform & ) throw(); //!< shared copy !
			~Platform() throw();
			
			const Core::Platform * operator->() const throw();
			
		private:
			Core::Platform::Pointer pPlatform_;
			Platform( const cl_platform_id platform_id );
			YOCTO_DISABLE_ASSIGN(Platform);
			friend class Driver;
		};
	}
	
}

#endif
