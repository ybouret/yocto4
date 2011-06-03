#ifndef YOCTO_THREADING_MUTEX_INCLUDED
#define YOCTO_THREADING_MUTEX_INCLUDED 1

#include "yocto/lockable.hpp"

#if defined(YOCTO_BSD)
#	if !defined(__USE_UNIX98)
#		define __USE_UNIX98
#	endif
#	include <pthread.h>
#endif /* YOCTO_BSD */

#if defined(YOCTO_WIN)
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#endif /* YOCTO_WIN */


namespace yocto 
{
	namespace threading
	{
		//! recursive mutex
		class mutex : public lockable
		{
		public:
			explicit mutex( const char * ) throw();
			virtual ~mutex() throw();
			
			virtual void lock()     throw();
			virtual void unlock()   throw();
			virtual bool try_lock() throw();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(mutex);
#if	defined(YOCTO_BSD)
			pthread_mutex_t mutex_;
#endif
			
#if	defined(YOCTO_WIN)
			CRITICAL_SECTION mutex_;
#endif
		public:
			char name[32];
			
		};
	}
	
}

#endif
