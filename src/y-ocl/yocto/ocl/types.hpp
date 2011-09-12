#ifndef YOCTO_OCL_TYPES_INCLUDED
#define YOCTO_OCL_TYPES_INCLUDED 1

#include "yocto/os.hpp"

#if defined(YOCTO_APPLE)
#	include <OpenCL/cl.h>
#else
#	include <CL/cl.h>
#endif

namespace yocto
{
	namespace ocl
	{
		namespace Core
		{
			
			const char * StrErr( const cl_int err ) throw(); //!< convert errors to string
			
		}
		
	}
	
}

#endif
