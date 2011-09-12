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
	
		namespace Core
		{
			const char * StrErr(const cl_int err) throw();
			
			typedef memory::pooled        MemoryType;
			typedef MemoryType::allocator Allocator;
			
			template <typename T> struct ArrayOf { typedef memory::buffer_of<T,MemoryType> Type; };
			
			
		}
		
		typedef Core::ArrayOf<char*>::Type Extensions; 
		
	}

	
}

#endif
