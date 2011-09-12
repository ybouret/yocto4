#include "yocto/ocl/buffer.hpp"
#include "yocto/ocl/exception.hpp"

namespace yocto
{
	
	namespace ocl
	{
		
		namespace Core
		{
			Buffer:: Buffer( cl_mem buf ) :
			MemObject( buf )
			{
			}
			
			Buffer:: ~Buffer() throw() {}
		}
		
		Buffer:: ~Buffer() throw() {}
		
		
		static inline
		cl_mem __create_buffer(cl_context context, 
							   cl_mem_flags    flags,
							   size_t          size,
							   void *          host_ptr )
		{
			cl_int err = CL_SUCCESS;
			cl_mem m   = clCreateBuffer(context, flags, size, host_ptr, &err);
			if( CL_SUCCESS != err )
				throw Exception( err, "clCreateBuffer" );
			return m;
		}
		
		
		Buffer:: Buffer(const Context  &context,
						cl_mem_flags    flags,
						size_t          size,
						void *          host_ptr ) :
		MemObject( __create_buffer( *context, flags, size, host_ptr ) ),
		pBuf_( new Core::Buffer( *hMem_ ) )
		{
			
		}
		
		Buffer:: Buffer( const Buffer &buf ) throw() :
		MemObject( buf ),
		pBuf_( buf.pBuf_ )
		{}
		
		const Core::Buffer * Buffer:: operator->() const throw()
		{
			return & (*pBuf_);
		}
		
		
		
	}
	
}

#include "yocto/ocl/kernel.hpp"
namespace yocto
{
	
	namespace ocl
	{
		void Kernel:: SetArg( cl_uint arg_index, Buffer & __memobj )
		{
			SetArg( arg_index, sizeof(cl_mem), (void *) & ( __memobj->ASSOCIATED_MEMOBJECT ) );
		}
	}
}


