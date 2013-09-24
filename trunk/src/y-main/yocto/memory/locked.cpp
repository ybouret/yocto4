#include "yocto/memory/locked.hpp"

#if defined(YOCTO_BSD)
#include <unistd.h>
#include <cerrno>
#include <sys/mman.h>
#include <cstdlib>
#endif

#if defined(YOCTO_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstring>
#endif

#include "yocto/exceptions.hpp"
#include "yocto/object.hpp"

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
        
        
        void * locked:: acquire( size_t &n )
        {
            YOCTO_LOCK(access);
            if(n>0)
            {
                //--------------------------------------------------------------
                // rounding #num pages
                //--------------------------------------------------------------
                size_t num_pages = n/page_size;
                if( 0 != (n%page_size) ) ++num_pages;
                const size_t m   = num_pages * page_size;
                if(m<n) throw exception("locked::acquire overflow");
                
#if defined(YOCTO_BSD)
                void *addr = calloc(1, m);
                //-- phase 1: get memory
                if(!addr)
                {
                    n = 0;
                    throw libc::exception( errno, "locked::acquire(%u)", unsigned(m));
                }
                
                //-- phase 2: lock memory
                if( mlock(addr, m) != 0 )
                {
                    free(addr);
                    n = 0;
                    throw libc::exception( errno, "mlock");
                }
                
                n = m;
                return addr;
#endif
                
#if defined(YOCTO_WIN)
				//-- phase 1: get VirtualMemory
				void *addr = ::VirtualAlloc( 0, m, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
				if(!addr)
				{
					n=0;
					throw win32::exception( ::GetLastError(), "VirtualAlloc" );
				}
				
				//-- phase 2: lock Virtual Memory
				if( ! ::VirtualLock(addr,m) )
				{
					const DWORD err = ::GetLastError();
					n = 0;
					::VirtualFree(addr,0,MEM_RELEASE);
					throw win32::exception( err, "VirtualLock" );
				}
				n=m;
				return addr;
#endif
            }
            else
                return 0;
        }
        
        
        void * locked:: try_acquire(size_t &n, bool &safe)
        {
            YOCTO_LOCK(access);
            safe = false;
            if(n>0)
            {
                //--------------------------------------------------------------
                // rounding #num pages
                //--------------------------------------------------------------
                size_t num_pages = n/page_size;
                if( 0 != (n%page_size) ) ++num_pages;
                const size_t m   = num_pages * page_size;
                if(m<n) throw exception("locked::try_acquire overflow");
                
#if defined(YOCTO_BSD)
                void *addr = calloc(1, m);
                //-- phase 1: get memory
                if(!addr)
                {
                    n = 0;
                    throw libc::exception( errno, "locked::try_acquire(%u)", unsigned(m));
                }
                
                //-- phase 2: lock memory
                safe = true;
                if( mlock(addr, m) != 0 )
                {
                    safe = false;
                }
                
                n = m;
                return addr;
#endif
                
#if defined(YOCTO_WIN)
				//-- phase 1: get VirtualMemory
				void *addr = ::VirtualAlloc( 0, m, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
				if(!addr)
				{
					n=0;
					throw win32::exception( ::GetLastError(), "VirtualAlloc" );
				}
				
				//-- phase 2: lock Virtual Memory
                safe = true;
				if( ! ::VirtualLock(addr,m) )
				{
                    safe = false;
				}
				n=m;
				return addr;
#endif
            }
            else
                return 0;

        }
        
        void locked:: release(void *&p, size_t &n) throw()
        {
            YOCTO_LOCK(access);
            if(n>0)
            {
                assert(p!=0);
#if defined(YOCTO_BSD)
                munlock(p, n);
                free(p);
#endif
                
#if defined(YOCTO_WIN)
				(void)::VirtualUnlock(p,n);
				::VirtualFree(p,0,MEM_RELEASE);
#endif
                p = 0;
                n = 0;
            }
        }
        
    }
}

