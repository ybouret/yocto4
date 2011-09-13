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
		void 
#if defined(YOCTO_WIN)
		WINAPI
#endif
		__notify(const char *errinfo, 
				 const void *private_info, 
				 size_t      cb, 
				 void       *user_data ) throw()
		{
			assert( user_data != NULL );
			const Context             *ctx = static_cast<Context *>(user_data);
			const memory::local_buffer errdata( private_info, cb );
			ctx->Notify(errinfo, errdata);
		}
		
		static inline
		cl_context __create_context( const DeviceIDs &devices, Context *from)
		{

			cl_int     err = CL_SUCCESS;
			cl_context ctx = clCreateContext(NULL, devices.size, devices(), __notify, from, &err);
			if( CL_SUCCESS != err )
				throw Exception( err, "clCreateContext(#devices=%u)", unsigned(devices.size) );
			return ctx;
		}
		
		
		
		Context:: Context(  const DeviceIDs &device_list  ) :
		Shared<cl_context>( __create_context( device_list, this) ),
		NUM_DEVICES( device_list.size ),
		devices(    NUM_DEVICES       ),
		DEVICES( devices()            )
		{
			size_t len = 0;
			cl_int err = clGetContextInfo( *(*this), CL_CONTEXT_DEVICES, 0, NULL, &len);
			
			if( err != CL_SUCCESS )
				throw Exception( err, "clGetContextInfo(DEVICES length)");
			
			if( len != devices.bytes )
				throw Exception( CL_INVALID_VALUE, "CL_CONTEXT_DEVICES mismatch" );
			
			err = clGetContextInfo( *(*this), CL_CONTEXT_DEVICES, devices.bytes, (void*)devices(), NULL);
			if( err != CL_SUCCESS )
				throw Exception( err, "clGetContextInfo(DEVICES)" );
		}
		
		void Context:: Notify( const char *errinfo, const memory::ro_buffer &errdata ) const throw()
		{
			assert( errinfo != NULL );
			std::cerr << "[Notify][" << errinfo << "]" << std::endl;
		}
	}
}
