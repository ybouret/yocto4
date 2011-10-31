
// thread placement is not supported on MacOSX
#if defined(_WIN32)
#	include "yocto/code/utils.hpp"
#else
#	if defined(__GNUC__)
#		if defined(__linux__)
#			define YOCTO_CPU_SET_PTHREAD 1
#			define YOCTO_CPU_SET cpu_set_t
#		endif
#		if defined(__FreeBSD__)
#			include <pthread_np.h>
#			define YOCTO_CPU_SET_PTHREAD 1
#			define YOCTO_CPU_SET cpuset_t
#		endif
#	endif
#endif

#if defined(_WIN32) || defined(YOCTO_CPU_SET_PTHREAD)
#	define YOCTO_THREAD_AFFINITY 1
#endif

#include "yocto/threading/team.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/threading/thread.hpp"
#include "yocto/hw.hpp"

#include <iostream>

namespace yocto
{
	namespace threading
	{
		
		//----------------------------------------------------------------------
		// Win32 API wrapper
		//----------------------------------------------------------------------
#if defined(_WIN32)
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
		static  void __assign( thread::handle_t h, const size_t j )
		{
			YOCTO_CPU_SET cpu_set;
			CPU_ZERO(  &cpu_set );
			CPU_SET(j, &cpu_set );
			const int err = pthread_setaffinity_np( h, sizeof(YOCTO_CPU_SET), &cpu_set );
			if( err != 0 )
				throw libc::exception( err, "pthread_setaffinity_np" );
		}
#endif
	
		
#include "./team-member.hxx"
		
		static inline size_t cpu_index(const size_t        i,
									   const team::layout &l,
									   const size_t        n ) throw()
		{
			const size_t j =  ( l.root + i % l.size ) % n;
			std::cerr << " |_ assign on CPU #" << j << "/" << n << std::endl;
			return j;
		}
		
		void team:: place(  )
		{
			assert( size > 0 );
#if defined(YOCTO_THREAD_AFFINITY)
			const size_t  cpu_setsize = hardware::nprocs();
			member       *m           = static_cast<member *>(wksp_);
			//------------------------------------------------------------------
			// assign main thread
			//------------------------------------------------------------------
			std::cerr << "-- main thread: " << std::endl;
			__assign( thread::get_current_handle(), cpu_index( 0, *this, cpu_setsize ) );
			
			//------------------------------------------------------------------
			// assign team thread
			//------------------------------------------------------------------
			std::cerr << "-- members    : " << std::endl;
			for( size_t i=0; i < size; ++i )
				__assign(  m[i].thr.get_handle(), cpu_index( i, *this, cpu_setsize ) );
#endif
			
			
		}
	
		
	}
}
