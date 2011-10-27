
// thread placement is not supported on MacOSX
#if defined(_WIN32)
#	include "yocto/code/utils.hpp"
#else
#	if defined(__GNUC__)
#		if defined(__linux__)
#			define YOCTO_CPU_SET_PTHREAD 1
#		endif
#	endif
#endif

#include "yocto/threading/team.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/threading/thread.hpp"

namespace yocto
{
	namespace threading
	{
		
#if defined(_WIN32)
		static inline DWORD __numCPU()
		{
			SYSTEM_INFO sysinfo;
			::GetSystemInfo( &sysinfo );
			const DWORD n = sysinfo.dwNumberOfProcessors;
			if( n <= 0 ) throw exception("sysinfo.dwNumberOfProcessors=0");
			return n;
		}
#endif
#include "./team-member.hxx"
		
		void team:: place( size_t cpu_start, size_t cpu_count )
		{
			if(cpu_count<=0)
				throw exception("team.place(cpu_count=0)");
#if defined(YOCTO_CPU_SET_PTHREAD)
			const size_t cpu_setsize = CPU_SETSIZE;
			if( cpu_setsize <= 0 )
				throw exception("CPU_SETSIZE=0");
			cpu_set_t  cpu_set;
			size_t     cpu_idx = cpu_start;
			member    *m       = static_cast<member *>(wksp_);
			for( size_t i=0; i < size; ++i )
			{
				const size_t j = (cpu_idx++ % cpu_count) % cpu_setsize;
				CPU_ZERO(  &cpu_set );
				CPU_SET(j, &cpu_set );
				const int err = pthread_setaffinity_np( m[i].thr.get_handle(), sizeof(cpu_set_t), &cpu_set );
				if( err != 0 )
					throw libc::exception( err, "pthread_setaffinity_np" );
			}
#endif
			
#if defined(_WIN32)
			const DWORD cpu_setsize = __numCPU(); // TODO: <= 32 ?
			size_t      cpu_idx = cpu_start;
			member    *m       = static_cast<member *>(wksp_);
			for( size_t i=0; i < size; ++i )
			{
				const size_t j = (cpu_idx++ % cpu_count) % cpu_setsize;
				const DWORD_PTR mask = DWORD_PTR(1) << j;
				if( ! ::SetThreadAffinityMask( m[i].thr.get_handle(), mask ) )
				{
					const DWORD err = ::GetLastError();
					throw win32::exception( err, "::SetThreadAffinityMask" );
				}
			}
#endif
			
		}
		
		void team:: flat()
		{
#if defined(YOCTO_CPU_SET_PTHREAD)
			place(0,CPU_SETSIZE);
#endif
			
#if defined(_WIN32)
			place(0,__numCPU());
#endif
		}
		
	}
}
