#ifndef YOCTO_OCL_TYPES_INCLUDED
#define YOCTO_OCL_TYPES_INCLUDED 1

#include "yocto/memory/buffers.hpp"
#include "yocto/memory/records.hpp"
#include "yocto/memory/pooled.hpp"
#include "yocto/string.hpp"

#if defined(YOCTO_APPLE)
#	include <OpenCL/cl.h>
#else
#	include	<CL/cl.h>
#endif

namespace yocto
{
	
	namespace ocl
	{
		
		
		const char * StrErr(const cl_int err) throw();
		
		typedef memory::pooled                   MemoryType;
		typedef MemoryType::allocator            Allocator;
		template <typename T> struct             ArrayOf { typedef memory::buffer_of<T,MemoryType> Type; };
		typedef ArrayOf<char*>::Type             Extensions; 
	
		typedef memory::records_of<cl_device_id> DeviceMap;
		typedef ArrayOf<cl_device_id>::Type      DeviceIDs;
		
		//! take care of OpenCL reference counted structs
		template <typename T>
		class Shared
		{
		public:
			YOCTO_ARGUMENTS_DECL_T;
			
			typedef cl_int (CL_API_CALL *Modifier)(T); //!< clRetainT|clReleaseT
			typedef size_t (            *RCounter)(T); //!< get reference count
			
			inline   Shared( T h ) throw() : handle_( h )
			{
				assert( NULL != handle_ );
			}
			
			inline   Shared( const Shared &other ) throw() : handle_( other.handle_ )
			{
				assert( NULL != handle_ );
				(void) Retain_( handle_ );
			}
			
			inline virtual ~Shared() throw()
			{
				assert( handle_ );
				(void) Release_(handle_);
			}
			
			inline type       & operator*() throw()       { assert(handle_); return handle_; }
			inline const_type & operator*() const throw() { assert(handle_); return handle_; }
			
			inline size_t ref_count() const { return RefCount_( handle_ ); }
			
		private:
			YOCTO_DISABLE_ASSIGN( Shared );
			T handle_;
			
			static const Modifier Retain_;   //!< to be set
			static const Modifier Release_;  //!< to be set
			static const RCounter RefCount_; //!< to be set
		};
		
#define YOCTO_OPENCL_SHARED_IMPL(CL_TYPE,RETAIN,RELEASE,REFCOUNT)                         \
/*   */ template <> const Shared<CL_TYPE>::Modifier Shared<CL_TYPE>::Retain_   = RETAIN;  \
/*   */ template <> const Shared<CL_TYPE>::Modifier Shared<CL_TYPE>::Release_  = RELEASE; \
/*   */ template <> const Shared<CL_TYPE>::RCounter Shared<CL_TYPE>::RefCount_ = REFCOUNT
		
		
	
		
	}
	
	
}

#endif
