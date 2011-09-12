#include "yocto/ocl/platform.hpp"
#include "yocto/ocl/_query.hpp"

#include <iostream>

namespace yocto
{
	
	namespace ocl
	{
		
		namespace Core 
		{
			
#define YCL_PLATFORM(X) X( GetInfoString( clGetPlatformInfo, id, CL_PLATFORM_##X, "CL_PLATFORM_" #X ) )
			
			Platform:: ~Platform() throw()
			{
			}
			
#if 0
			cl_int clGetDeviceIDs (cl_platform_id platform,
								   cl_device_type device_type,
								   cl_uint num_entries,
								   cl_device_id *devices,
								   cl_uint *num_devices);
#endif
			
			static inline cl_uint __GetDeviceIDsCount( cl_platform_id platform_id )
			{
				cl_uint      ans = 0;
				const cl_int err = clGetDeviceIDs( platform_id, CL_DEVICE_TYPE_ALL, 0, NULL, &ans );
				if( CL_SUCCESS != err || ans <= 0 ) throw Exception( err, "clGetDeviceIDs(count)" );
				return ans;
			}
			
			static inline void __GetDeviceIDsValue( cl_platform_id platform_id, const cl_uint num_devices, cl_device_id *devices )
			{
				assert( devices );
				assert( num_devices > 0 );
				cl_uint chk = 0;
				const cl_int err = clGetDeviceIDs( platform_id, CL_DEVICE_TYPE_ALL, num_devices, devices, &chk );
				if( CL_SUCCESS != err || num_devices != chk ) throw Exception( err, "clGetDevicesIDs(value)" );
				
			}
			
			Platform:: Platform( const cl_platform_id __id ) :
			id( __id ),
			YCL_PLATFORM(NAME),
			num_devices( __GetDeviceIDsCount( id ) ),
			device_id( num_devices ),
			device_( num_devices  ),
			device( device_(0), device_.size ),
			YCL_PLATFORM(PROFILE),
			YCL_PLATFORM(VERSION),
			YCL_PLATFORM(VENDOR),
			YCL_PLATFORM(EXTENSIONS),
			extension( CountExtensions( EXTENSIONS ) )
			{
				
				//--------------------------------------------------------------
				// load device ids
				//--------------------------------------------------------------
				__GetDeviceIDsValue( id, num_devices, (cl_device_id *)device_id(0) );
				
				//--------------------------------------------------------------
				// Create Devices
				//--------------------------------------------------------------
				assert( 0 == device.size );
				assert( num_devices == device.maxi );
				for( cl_uint i=0; i < num_devices; ++i )
				{
					const cl_device_id dev = device_id[i]; assert( dev );
					const ocl::Device  D( dev );
					(*(Core::Devices *)&device).append( D );
				}
				assert( num_devices == device.size );
				
				//--------------------------------------------------------------
				// Parse Extensions
				//--------------------------------------------------------------
				ParseExtensions(EXTENSIONS, extension );
			}
			
			
			cl_device_id Platform:: DefaultDeviceID() const
			{
				cl_device_id _id = NULL;
				const cl_int err = clGetDeviceIDs(this->id,CL_DEVICE_TYPE_DEFAULT, 1, & _id, NULL);
				if( CL_SUCCESS != err )
					throw Exception( err, "clGetDeviceIDs DEFAULT");
				return _id;
			}
			
			
		}
		
		Platform:: Platform( const cl_platform_id __id ) :
		pPlatform_( new Core::Platform( __id ) )
		{
		}
		
		Platform:: Platform( const Platform & other ) throw() :
		pPlatform_( other.pPlatform_ )
		{
			
		}
		
		
		Platform:: ~Platform() throw()
		{
			
		}
		
		const Core::Platform * Platform:: operator->() const throw()
		{
			return & (*pPlatform_);
		}
		
		
	}
	
}
