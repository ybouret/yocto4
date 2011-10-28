
// thread placement is not supported on MacOSX
#if defined(_WIN32)
#	include "yocto/code/utils.hpp"
#else
#	if defined(__GNUC__)
#		if defined(__linux__)
#			include <sys/sysinfo.h>
#			define YOCTO_CPU_SET_PTHREAD 1
#		endif
#	endif
#endif

#if defined(_WIN32) || defined(YOCTO_CPU_SET_PTHREAD)
#	define YOCTO_THREAD_AFFINITY 1
#endif

#include "yocto/threading/team.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/threading/thread.hpp"

#include <iostream>

namespace yocto
{
	namespace threading
	{
		
		//----------------------------------------------------------------------
		// Win32 API wrapper
		//----------------------------------------------------------------------
#if defined(_WIN32)
		static inline size_t __numCPU()
		{
			SYSTEM_INFO sysinfo;
			::GetSystemInfo( &sysinfo );
			const DWORD n = sysinfo.dwNumberOfProcessors;
			if( n <= 0 ) throw exception("sysinfo.dwNumberOfProcessors=0");
			return n;
		}
		
		static inline void __assign( thread::handle_t h, const size_t j )
		{
			const DWORD_PTR mask = DWORD_PTR(1) << j;
			if( ! ::SetThreadAffinityMask( h, mask ) )
			{
				const DWORD err = ::GetLastError();
				throw win32::exception( err, "::SetThreadAffinityMask" );
			}
		}
#endif
		
		//----------------------------------------------------------------------
		// pthread wrapper
		//----------------------------------------------------------------------
#if defined(YOCTO_CPU_SET_PTHREAD)
#	if defined (__linux__)
		static inline size_t __numCPU() 
		{
			const int n = get_nprocs();
			if( n <= 0 ) 
				throw exception("No CPU Online!");
			return n;
		}
#endif
		
		static  void __assign( thread::handle_t h, const size_t j )
		{
			cpu_set_t  cpu_set;
			CPU_ZERO(  &cpu_set );
			CPU_SET(j, &cpu_set );
			const int err = pthread_setaffinity_np( h, sizeof(cpu_set_t), &cpu_set );
			if( err != 0 )
				throw libc::exception( err, "pthread_setaffinity_np" );
		}
#endif
		
		
#include "./team-member.hxx"
		
		static inline size_t cpu_index(const size_t cpu_idx, 
									   const size_t cpu_count, 
									   const size_t cpu_setsize ) throw()
		{
			const size_t j = ( cpu_idx % cpu_count ) % cpu_setsize;
			std::cerr << "-- assign on CPU #" << j << "/" << cpu_setsize << std::endl;
			return j;
		}
		
		void team:: place( size_t cpu_start, size_t cpu_count )
		{
			if(cpu_count<=0)
				throw exception("team.place(cpu_count=0)");
			
#if defined(YOCTO_THREAD_AFFINITY)
			const size_t cpu_setsize = __numCPU();
			member       *m          = static_cast<member *>(wksp_);
			size_t        cpu_idx    = cpu_start;
			//------------------------------------------------------------------
			// assign main thread
			//------------------------------------------------------------------
			__assign( thread::get_current_handle(), cpu_index( cpu_idx, cpu_count, cpu_setsize ) );
			
			//------------------------------------------------------------------
			// assign team thread
			//------------------------------------------------------------------
			for( size_t i=0; i < size; ++i )
				__assign(  m[i].thr.get_handle(), cpu_index( cpu_idx++, cpu_count, cpu_setsize ) );
#endif
			
			
		}
		
		void team:: flat()
		{
#if defined(YOCTO_THREAD_AFFINITY)
			place(0,__numCPU());
#endif
		}
		
	}
}
