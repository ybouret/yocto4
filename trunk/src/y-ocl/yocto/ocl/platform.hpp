#ifndef YOCTO_PLATFORM_INCLUDED
#define YOCTO_PLATFORM_INCLUDED 1

#include "yocto/ocl/types.hpp"

namespace yocto
{
	
	namespace ocl
	{
	
		class Platform
		{
		public:
			Platform( cl_platform_id platform);
			~Platform() throw();
			
			const string PROFILE;    //!< CL_PLATFORM_PROFILE
			const string VERSION;    //!< CL_PLATFORM_VERSION
			const string NAME;       //!< CL_PLATFORM_NAME
			const string VENDOR;     //!< CL_PLATFORM_VENDOR
			const string EXTENSIONS; //!< CL_PLATFORM_EXTENSIONS
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(Platform);
			
		};
		
	}
	
}

#endif
