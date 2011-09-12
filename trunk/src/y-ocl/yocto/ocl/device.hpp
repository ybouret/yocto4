#ifndef YOCTO_OPENCL_DEVICE_INCLUDED
#define YOCTO_OPENCL_DEVICE_INCLUDED 1

#include "yocto/ocl/types.hpp"

namespace yocto 
{
	
	namespace ocl
	{
		
		namespace Core 
		{
			typedef ArrayOf<size_t>::Type Sizes;
			
			class Device : public Object
			{
			public:
				const cl_device_id   id;
				const cl_platform_id PLATFORM;                  //!< CL_DEVICE_PLATFORM
				const cl_device_type TYPE;                      //!< CL_DEVICE_TYPE
				const cl_uint        VENDOR_ID;                 //!< CL_DEVICE_VENDOR_ID
				const cl_uint        MAX_COMPUTE_UNITS;         //!< CL_DEVICE_MAX_COMPUTE_UNITS
				
				const cl_uint        MAX_WORK_ITEM_DIMENSIONS;  //!< CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS
				const Sizes          MAX_WORK_ITEM_SIZES;       //!< CL_DEVICE_MAX_WORK_ITEM_SIZES
				const size_t         MAX_WORK_GROUP_SIZE;       //!< CL_DEVICE_MAX_WORK_GROUP_SIZE
				
				const cl_uint        MAX_CLOCK_FREQUENCY;       //!< CL_DEVICE_MAX_CLOCK_FREQUENCY
				
				const cl_uint        ADDRESS_BITS;              //!< CL_DEVICE_ADDRESS_BITS
				
				const cl_bool        IMAGE_SUPPORT;             //!< CL_DEVICE_IMAGE_SUPPORT
				const cl_uint        MAX_READ_IMAGE_ARGS;       //!< CL_DEVICE_MAX_READ_IMAGE_ARGS
				const cl_uint        MAX_WRITE_IMAGE_ARGS;      //!< CL_DEVICE_MAX_WRITE_IMAGE_ARGS
				const size_t         IMAGE2D_MAX_WIDTH;         //!< CL_DEVICE_IMAGE2D_MAX_WIDTH
				const size_t         IMAGE2D_MAX_HEIGHT;        //!< CL_DEVICE_IMAGE2D_MAX_HEIGHT
				const size_t         IMAGE3D_MAX_WIDTH;         //!< CL_DEVICE_IMAGE3D_MAX_WIDTH
				const size_t         IMAGE3D_MAX_HEIGHT;        //!< CL_DEVICE_IMAGE3D_MAX_HEIGHT
				const size_t         IMAGE3D_MAX_DEPTH;         //!< CL_DEVICE_IMAGE3D_MAX_DEPTH
				const cl_uint        MAX_SAMPLERS;              //!< CL_DEVICE_MAX_SAMPLERS
				
				const cl_ulong       MAX_MEM_ALLOC_SIZE;        //!< CL_DEVICE_MAX_MEM_ALLOC_SIZE
				const cl_ulong       GLOBAL_MEM_SIZE;           //!< CL_DEVICE_GLOBAL_MEM_SIZE
				const cl_ulong       LOCAL_MEM_SIZE;            //!< CL_DEVICE_LOCAL_MEM_SIZE
				
				const string         NAME;                      //!< CL_DEVICE_NAME
				const string         VENDOR;                    //!< CL_DEVICE_VENDOR
				const string         VERSION;                   //!< CL_DEVICE_VERSION
				const string         DRIVER;                    //!< CL_DRIVER_VERSION
				const string         PROFILE;                   //!< CL_DEVICE_PROFILE
			private:
				string               EXTENSIONS;                //!< CL_DEVICE_EXTENSIONS, will be parsed
			public:
				const Extensions     extension;                 //!< parsed
				const string         nickname;
				
				explicit Device( cl_device_id __id );
				virtual ~Device() throw();
				
				typedef Proxy<Device> Pointer;
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(Device);
			};
		}
		
		
		class Device 
		{
		public:
			Device( const cl_device_id );
			~Device() throw();
			Device( const Device & ) throw(); //!< shared copy
			
			const Core::Device * operator->() const throw();
			
		private:
			Core::Device::Pointer pDevice_;
			YOCTO_DISABLE_ASSIGN(Device);
		};
		
		
		
	}
	
}

#endif
