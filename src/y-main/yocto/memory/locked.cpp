#include "yocto/memory/locked.hpp"

#if defined(YOCTO_BSD)
#include <unistd.h>
#include <cerrno>
#endif

#if defined(YOCTO_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstring>
#endif

#include "yocto/exceptions.hpp"

namespace yocto
{
    namespace memory
    {
        
        locked:: ~locked() throw()
        {
        }
        
        static inline
        size_t __get_page_size()
        {
#if defined(YOCTO_BSD)
            //const int ans = ::getpagesize();
            const long ans = sysconf(_SC_PAGESIZE);
            if(ans<=0)
                throw libc::exception( errno, "get system PAGE_SIZE");
            return ans;
#endif

#if defined(YOCTO_WIN)
			SYSTEM_INFO sSysInfo;         // useful information about the system
			memset(&sSysInfo,0,sizeof(SYSTEM_INFO));
			GetSystemInfo(&sSysInfo);     // no error get info
			const DWORD ans = sSysInfo.dwPageSize;
			if(ans<=0) throw win32::exception( ERROR_INVALID_DATA, "SYSTEM_INFO.dwPageSize");
			return ans;
#endif
        }
        
        locked:: locked() : page_size( __get_page_size() )
        {
        }
        
        const char locked::name[] = "locked memory";
        
        
    }
}

