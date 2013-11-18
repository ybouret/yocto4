#include "yocto/sys/hw.hpp"

#if defined(YOCTO_APPLE) 
#include <sys/param.h>
#include <sys/sysctl.h>
#endif

#if defined(YOCTO_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#if defined(YOCTO_LINUX) || defined(YOCTO_FREEBSD) || defined(YOCTO_SUNOS)
#include <unistd.h>
#endif

namespace yocto
{
	
	size_t hardware:: nprocs()
	{
		
#if defined(YOCTO_APPLE) 
		int      nm[2] = { CTL_HW, HW_AVAILCPU };
		size_t   len = 4;
		uint32_t count=0;
		sysctl(nm, 2, &count, &len, NULL, 0);
		
		if(count < 1) {
			nm[1] = HW_NCPU;
			sysctl(nm, 2, &count, &len, NULL, 0);
			if(count < 1) { count = 1; }
		}
		return count;
#endif
		
#if defined(YOCTO_WIN)
		SYSTEM_INFO sysinfo;
		::GetSystemInfo( &sysinfo );
		const DWORD n = sysinfo.dwNumberOfProcessors;
		return n <= 1 ? 1:n;
#endif
		
#if defined(YOCTO_LINUX) || defined(YOCTO_FREEBSD) || defined(YOCTO_SUNOS)
		const long n = sysconf( _SC_NPROCESSORS_ONLN );
		return n <= 0  ? 1 : n;
#endif
		
	}
	
}
