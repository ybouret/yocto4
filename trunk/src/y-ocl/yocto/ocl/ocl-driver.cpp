#include "yocto/ocl/driver.hpp"
#include "yocto/ocl/exception.hpp"

namespace yocto
{
	
	namespace ocl
	{
		
		const char Driver::name[] = "OpenCL";
		Driver:: ~Driver() throw() {}
		
		static inline cl_uint __num_platforms()
		{
			cl_uint      num_platforms = 0;
			const cl_int err = clGetPlatformIDs( 0, NULL, &num_platforms );
			if( CL_SUCCESS != err )
				throw Exception( err, "clGetPlatformIDs(&num_platforms)" );
			return num_platforms;
		}
		
		
		static inline void __build_platform( void *ptr, void *args )
		{
			assert(ptr!=NULL);
			assert(args!=NULL);
			const cl_platform_id p_id = *(cl_platform_id *)args;
			new (ptr) Platform( p_id );
		}
		
		Driver:: Driver( ) :
		num_platforms( __num_platforms() ),
		platformIDs( num_platforms ),
		platforms_( num_platforms  ),
		platforms( platforms_(), num_platforms ),
		num_devices(0)
		{
			//==================================================================
			//
			// Load cl_platform_id
			//
			//==================================================================
			const cl_int err = clGetPlatformIDs( num_platforms, (cl_platform_id *) platformIDs(), NULL);
			if( CL_SUCCESS != err ) 
				throw Exception( err, "clGetPlatformIDs(%u,platforms)", num_platforms);
			
			//==================================================================
			//
			// Build All Platforms
			//
			//==================================================================
			for( cl_uint i=0; i < num_platforms; ++i ) 
			{
				((memory::records_of<Platform>&)platforms)( __build_platform, (void*) & platformIDs[i] );
				(size_t &) num_devices += platforms[i].num_devices;	
				
			}
			assert( num_platforms == platforms.size );
			
			
		}
		
		
		const Device & Driver:: operator[]( const cl_device_id device_id ) const
		{
			for( cl_uint i=0; i < num_platforms; ++i ) 
			{
				const Platform &P = platforms[i];
				for( cl_uint j=0; j < P.num_devices; ++j )
				{
					if( P.deviceIDs[j] == device_id) return P.devices[j];
				}
			}
			throw Exception( CL_INVALID_VALUE, "OpenCL[device_id]");
		}
		
	}
	
}