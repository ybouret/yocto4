#include "yocto/ocl/context.hpp"
#include "yocto/ocl/_query.hpp"

#include <iostream>

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
		
		static inline 
		void __notify(const char *errinfo, 
					  const void *private_info, 
					  size_t      cb, 
					  void       *user_data )
		{
			assert( user_data != NULL );
			const Context *ctx = static_cast<Context *>(user_data);
			ctx->Notify(errinfo);
		}
		
		static inline
		cl_context __create_context( cl_uint num_devices, cl_device_id *devices, Context *from)
		{
			if( num_devices <= 0 || NULL == devices)
				throw Exception( CL_INVALID_VALUE, "No Devices for Context" );
			
			cl_int     err = CL_SUCCESS;
			cl_context ctx = clCreateContext(NULL, num_devices, devices, __notify, from, &err);
			if( CL_SUCCESS != err )
				throw Exception( err, "clCreateContext()" );
			return ctx;
		}
		
		
		
		Context:: Context(  cl_uint num_devices, cl_device_id *devices  ) :
		Shared<cl_context>( __create_context( num_devices, devices, this) ),
		NUM_DEVICES( num_devices ),
		devices_(    NUM_DEVICES ),
		DEVICES( devices_()      )
		{
			size_t len = 0;
			cl_int err = clGetContextInfo( *(*this), CL_CONTEXT_DEVICES, 0, NULL, &len);
			
			if( err != CL_SUCCESS )
				throw Exception( err, "clGetContextInfo(DEVICES length)");
			
			if( len != devices_.bytes )
				throw Exception( CL_INVALID_VALUE, "CL_CONTEXT_DEVICES mismatch" );
			
			err = clGetContextInfo( *(*this), CL_CONTEXT_DEVICES, devices_.bytes, devices_(), NULL);
			if( err != CL_SUCCESS )
				throw Exception( err, "clGetContextInfo(DEVICES)" );
		}
		
		void Context:: Notify( const char *errinfo ) const throw()
		{
			assert( errinfo != NULL );
			std::cerr << "[Context.Notify][" << errinfo << "]" << std::endl;
		}
	}
}