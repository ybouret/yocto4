#include "yocto/ocl/platform.hpp"
#include "yocto/ocl/_query.hpp"

namespace yocto
{
	
	namespace ocl
	{

#define	YCLP_STR(NAME) NAME( Core::GetInfoString(clGetPlatformInfo, platform, CL_PLATFORM_ ## NAME, "CL_PLATFORM_" #NAME))

		Platform:: Platform( cl_platform_id platform ) :
		YCLP_STR(PROFILE),
		YCLP_STR(VERSION),
		YCLP_STR(NAME),
		YCLP_STR(VENDOR),
		YCLP_STR(EXTENSIONS),
		extensions( Core::CountExtensions(EXTENSIONS) )
		{
			Core::ParseExtensions( EXTENSIONS, extensions );
		}
		
		Platform:: ~Platform() throw() 
		{
		}
		
		
	}

}
