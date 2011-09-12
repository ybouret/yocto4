#ifndef YOCTO_OPENCL_EXCEPTION_INCLUDED
#define YOCTO_OPENCL_EXCEPTION_INCLUDED 1

#include "yocto/ocl/types.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
	
	namespace ocl
	{
		
		class Exception : public exception
		{
		public:
			explicit Exception( cl_int code, const char *fmt, ... ) throw() YOCTO_PRINTF_CHECK(3,4);
			virtual ~Exception() throw();
			Exception( const Exception &e ) throw();
			
			virtual const char *what() const throw();
			const   cl_int      code;
			
		private:
			YOCTO_DISABLE_ASSIGN(Exception);
			char      what_[ exception::max_length ];
		};
		
	}
	
}

#endif
