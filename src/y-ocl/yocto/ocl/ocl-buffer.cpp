#include "yocto/ocl/buffer.hpp"
#include "yocto/ocl/exception.hpp"

namespace yocto
{
	
	namespace ocl
	{
		
		Buffer:: ~Buffer() throw()
		{
		}
		
		
		static inline 
		cl_mem __create_buffer( const Context  &ctx, 
							   cl_mem_flags    flags,
							   size_t          size,
							   void           *host_ptr )
		{
			cl_int err = CL_SUCCESS;
			cl_mem buf = clCreateBuffer( *ctx, flags, size, host_ptr, &err);
			if( CL_SUCCESS != err )
				throw Exception( err, "clCreateBuffer()");
			
			return buf;
		}
		
		Buffer:: Buffer(const Context  &ctx, 
						cl_mem_flags    flags,
						size_t          size,
						void           *host_ptr ) :
		MemObject( __create_buffer( ctx, flags, size, host_ptr ) )
		{
			
		}
		
	}
	
}