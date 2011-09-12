#include "yocto/ocl/platform.hpp"
#include "yocto/ocl/_query.hpp"

namespace yocto
{
	
	namespace ocl
	{

		
		static inline cl_uint __num_devices( cl_platform_id platform )
		{
			cl_uint       num_devices = 0;
			const cl_int  err         = clGetDeviceIDs( platform, CL_DEVICE_TYPE_ALL, 0, NULL, &num_devices);
			if( CL_SUCCESS != err )
				throw Exception( err, "clGetDeviceIDs(&num_devices)" );
			return num_devices;
		}
		
#define	YCLP_STR(NAME) NAME( Core::GetInfoString(clGetPlatformInfo, platform, CL_PLATFORM_ ## NAME, "CL_PLATFORM_" #NAME))

		
		static inline void __build_device( void *ptr, void *args )
		{
			assert( ptr  != NULL );
			assert( args != NULL );
			new (ptr) Device( *(cl_device_id *)args );
		}
		
		Platform:: Platform( cl_platform_id platform ) :
		id(platform),
		num_devices( __num_devices(platform) ),
		deviceIDs( num_devices ),
		devices_( num_devices  ),
		devices( devices_(), num_devices ),
		YCLP_STR(PROFILE),
		YCLP_STR(VERSION),
		YCLP_STR(NAME),
		YCLP_STR(VENDOR),
		YCLP_STR(EXTENSIONS),
		extensions( Core::CountExtensions(EXTENSIONS) )
		{
			//== probe all the deviceIDs
			const cl_int err = clGetDeviceIDs( platform, CL_DEVICE_TYPE_ALL, num_devices, (cl_device_id *) deviceIDs(), NULL);
			if( CL_SUCCESS != err )
				throw Exception( err, "clGetDeviceIDs(num_devices)" );
		
			//== construct the associated devices
			for( cl_uint i=0; i < num_devices; ++i )
			{
				((memory::records_of<Device>&)devices)( __build_device, (void *)&deviceIDs[i] );
			}
			
			//== finalize by parsing extensions
			Core::ParseExtensions( EXTENSIONS, extensions );
		}
		
		Platform:: ~Platform() throw() 
		{
		}
		
		
	}

}
