#include "yocto/sys/pid.hpp"

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
    
    uint32_t get_process_h32()
    {
        const pid_type p = get_process_id();
        return hash32(&p, sizeof(p));
    }
	
}

