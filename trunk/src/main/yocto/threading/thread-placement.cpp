#include "yocto/threading/thread.hpp"
#include "yocto/exceptions.hpp"

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

		void thread:: foreach_cpu( thread::handle_t h, void (*proc)(size_t,void*), void *args)
		{
			assert(proc!=0);
			DWORD_PTR   cpu_set = 0;

			//-- try to fetch it
			for( size_t i=0; i < sizeof(DWORD_PTR) * 8; ++i )
			{
				const DWORD_PTR mask = DWORD_PTR(1) << i;
				const DWORD_PTR old  = ::SetThreadAffinityMask(h,mask);
				if( old != 0 )
				{
					//-- restore
					if( ! ::SetThreadAffinityMask(h,old) )
					{
						const DWORD err = ::GetLastError();
						throw win32::exception( err, "::SetThreadAffinityMask/Restore");
					}
					cpu_set = old;
				}
			}
			if( !cpu_set )
				throw win32::exception(ERROR_INVALID_PARAMETER,"Can't get Thread Affinity");

			//-- parse it
			for(size_t cpu_id = 0; cpu_id < sizeof(DWORD_PTR) * 8; ++cpu_id )
			{
				const DWORD_PTR mask = DWORD_PTR(1) << cpu_id;
				if( 0 != (cpu_set&mask) )
					proc(cpu_id,args);
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

		void thread:: foreach_cpu( thread::handle_t h, void (*proc)(size_t,void*), void *args)
		{
			assert(proc!=0);
			YOCTO_CPU_SET  cpu_set;
			CPU_ZERO( &cpu_set );
			const int err = pthread_getaffinity_np( h, sizeof(YOCTO_CPU_SET), &cpu_set);
			if( err != 0 )
				throw libc::exception( err, "pthread_getaffinity_np" );
			for( size_t cpu_id=0; cpu_id < CPU_SETSIZE; ++cpu_id )
			{
				if( CPU_ISSET(cpu_id, &cpu_set) )
				{
					proc(cpu_id,args);
				}
			}
		}
#endif

#if defined(YOCTO_THREAD_AFFINITY)
		void thread:: assign_cpu( thread::handle_t h, size_t cpu_id )
		{
			__assign(h,cpu_id);
		}
#else
		void thread:: assign_cpu( thread::handle_t, size_t )
		{

		}

		void thread:: foreach_cpu( thread::handle_t, void (*proc)(size_t,void*), void *)
		{
			assert(proc);
		}
#endif

		void thread::on_cpu( size_t cpu_id )
		{
			assign_cpu( handle, cpu_id);
		}

        void assign_current_thread_on( size_t cpu_id )
        {
            thread::assign_cpu( thread::get_current_handle(), cpu_id );
        }
        
	}

}
