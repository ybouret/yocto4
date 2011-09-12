#include "yocto/ocl/context.hpp"
#include "yocto/ocl/exception.hpp"

#include <iostream>


namespace yocto
{
	
	namespace ocl
	{
		namespace Core
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
			
			YOCTO_OPENCL_SHARED_IMPL(cl_context,clRetainContext,clReleaseContext, _ref_count);
			
			
			Context:: ~Context() throw()
			{
			}
			
			
			Context:: Context( cl_context /*context*/, size_t num_devices, const cl_device_id *devices  ) :
			device_id( num_devices ),
			NUM_DEVICES( device_id.size ),
			DEVICES( device_id(0) ) //,context_( context )
			{
				memcpy( (void*)device_id(0), devices, device_id.bytes );
			}
			
			
			static inline size_t __GetContextDevicesCount( cl_context context )
			{
				size_t       bytes = 0;
				const cl_int err   = clGetContextInfo (context,
													   CL_CONTEXT_DEVICES,
													   0,
													   NULL,
													   &bytes);
				if( err != CL_SUCCESS )
					throw Exception( err, "counting clGetContextInfo( CL_CONTEXT_DEVICES )" );
				
				assert( 0 == (bytes % sizeof(cl_device_id) ) );
				return bytes/sizeof(cl_device_id);
				
			}
			
			Context:: Context( cl_context context ) :
			device_id( __GetContextDevicesCount(context) ),
			NUM_DEVICES( device_id.size ),
			DEVICES(     device_id(0)   )//, context_( context )
			{
				//-- fetch THE device, OpenCL 1.0
				const cl_int err = clGetContextInfo (context,
													 CL_CONTEXT_DEVICES,
													 device_id.bytes,
													 (void*) device_id(0),
													 NULL);
				
				
				if( err != CL_SUCCESS )
					throw Exception( err, "clGetContextInfo( CL_CONTEXT_DEVICES )" );
				
			}
			
			
			
		} // Core
		
		cl_context Context:: __create_context( size_t num_devices, const cl_device_id *devices, void *self)
		{
			if( num_devices <= 0 ) throw exception("no devices to create context");
			if( devices == NULL  ) throw exception("NULL devices");
			
			Core::GlobalMemoryLock access;
			
			cl_int     err = CL_SUCCESS;
			cl_context ctx = clCreateContext(NULL, num_devices, devices, Context::Notify, self, &err);
			if( CL_SUCCESS != err )
				throw Exception( err, "clCreateContext" );
			return ctx;
		}
		
		
		Context:: Context( const Context &other ) throw() :
		hContext_( other.hContext_ ),
		pContext_( other.pContext_ )
		{
		}
		
		Context:: ~Context() throw() {}
		
		
		
#if defined( _MSC_VER)
#	pragma warning  (disable: 4355)
#endif
		
		Context:: Context( size_t num_devices, const cl_device_id *devices ) :
		hContext_(    __create_context(num_devices, devices, this) ),
		pContext_( new  Core::Context( *hContext_, num_devices, devices)       )
		{
		}
		
		Context:: Context( const Core::DeviceIDsMap &devmap ) :
		hContext_(  __create_context( devmap.size, devmap(0), this) ),
		pContext_( new  Core::Context( *hContext_, devmap.size, devmap(0))  )
		{
		}
		
		
		cl_context Context:: __create_context( cl_device_type device_type, void *self )
		{
			Core::GlobalMemoryLock access;
			cl_int     err = CL_SUCCESS;
			cl_context ctx =  clCreateContextFromType(NULL, device_type, Context::Notify, self, &err);
			if( CL_SUCCESS != err )
				throw Exception( err, "clCreateContextFromType" );
			return ctx;
		}
		
		Context:: Context( cl_device_type device_type ) :
		hContext_( __create_context( device_type, this ) ),
		pContext_( new Core::Context( *hContext_ )      )
		{
		}
		
		
		void  Context:: Notify(const char *errinfo,
							   const void *private_info,
							   size_t      cb,
							   void *      user_data)
		{
			assert( user_data );
			Context *Ctx = static_cast<Context *>( user_data );
			const memory::local_buffer prv( private_info, cb );
			Ctx->OnError( errinfo, length_of( errinfo ), prv );
		}
		
		
		void Context:: OnError(const char *errinfo, 
							   size_t /* errsize */,
							   const memory::ro_buffer & /*private_info*/ ) throw()
		{
			std::cerr << "OpenCL.Context.Notify:'" << errinfo << "'" << std::endl;
		}
		
		const Core::Context * Context:: operator->() const throw()
		{
			return & (* pContext_ );
		}
		
		
		const cl_context & Context:: operator*() const throw()
		{
			return * hContext_;
		}
		
	}
	
}
