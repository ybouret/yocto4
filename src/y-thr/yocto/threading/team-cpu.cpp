
// thread placement is not supported on MacOSX

#if defined(__GNUC__)
#	if defined(__linux__)
#		define YOCTO_CPU_SET_PTHREAD 1
//#		include <pthread.h>
#	endif
#endif

#include "yocto/threading/team.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/threading/thread.hpp"

namespace yocto
{
		namespace threading
		{
			
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
					const int err = pthread_setaffinity_np( m[i].thr.get_id(), sizeof(cpu_set_t), &cpu_set );
					if( err != 0 )
						throw libc::exception( err, "pthread_setaffinity_np" );
				}
				#endif
			}
			
			void team:: flat()
			{
				#if defined(YOCTO_CPU_SET_PTHREAD)
				place(0,CPU_SETSIZE);
				#endif
			}
			
		}
}
