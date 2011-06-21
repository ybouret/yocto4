#include "yocto/code/pid.hpp"

#if defined(YOCTO_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace yocto
{
	pid_type get_process_id()
	{
#if defined(YOCTO_BSD)
		return getpid();
#endif
		
#if defined(YOCTO_WIN)
		return ::GetCurrentProcessId();
#endif
		
	}
	
}

