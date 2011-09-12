#include "yocto/ocl/context.hpp"
#include "yocto/ocl/exception.hpp"

namespace yocto
{
	
	namespace ocl 
	{
		static inline
		size_t _ref_count( cl_context context )
		{
			cl_uint      ans = 0;
			const cl_int err = clGetContextInfo( context, CL_CONTEXT_REFERENCE_COUNT, sizeof(ans), &ans, NULL);
			if( err != CL_SUCCESS )
			{
				throw Exception( err, "clGetContextInfo( REF_COUNT )" );
			}
			return ans;
		}
		
		YOCTO_OPENCL_SHARED_IMPL(cl_context,clRetainContext,clReleaseContext,_ref_count);
		
		
		Context:: ~Context() throw() {}
		
		Context:: Context( cl_uint num_devices, cl_device_id *devices ) :
		Shared<cl_context>( NULL ),
		NUM_DEVICES( num_devices ),
		devices_(    num_devices ),
		DEVICES( devices_()      )
		{
			
		}
	}
}