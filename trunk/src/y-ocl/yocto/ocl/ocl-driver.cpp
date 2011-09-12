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
		
		
		Driver:: Driver( ) :
		num_platforms( __num_platforms() ),
		platforms( num_platforms )
		{
			//==================================================================
			//
			// Load cl_platform_id
			//
			//==================================================================
			const cl_int err = clGetPlatformIDs( num_platforms, (cl_platform_id *) platforms(), NULL);
			if( CL_SUCCESS != err ) 
				throw Exception( err, "clGetPlatformIDs(%u,platforms)", num_platforms);
			
			//==================================================================
			//
			// Build All Platforms
			//
			//==================================================================
			
		}
		
	}
	
}