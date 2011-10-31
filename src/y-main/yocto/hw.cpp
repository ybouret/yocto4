#include "yocto/hw.hpp"

#if YOCTO_APPLE == 1
#include <sys/param.h>
#include <sys/sysctl.h>
#endif

#if YOCTO_WIN == 1
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#if YOCTO_LINUX == 1 || YOCTO_FREEBSD == 1 || YOCTO_SUNOS == 1
#include <unistd.h>
#endif

namespace yocto
{
	
	size_t hardware:: nprocs()
	{
		
#if YOCTO_APPLE == 1
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
		
#if YOCTO_WIN
		SYSTEM_INFO sysinfo;
		::GetSystemInfo( &sysinfo );
		const DWORD n = sysinfo.dwNumberOfProcessors;
		return n <= 1 ? 1:n;
#endif
		
#if YOCTO_LINUX == 1 || YOCTO_FREEBSD == 1 || YOCTO_SUNOS == 1
		const long n = sysconf( _SC_NPROCESSORS_ONLN );
		return n <= 0  ? 1 : n;
#endif
		
	}
	
}
