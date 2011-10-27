#ifndef YOCTO_THREADING_THREAD_INCLUDED
#define YOCTO_THREADING_THREAD_INCLUDED 1

#include "yocto/threading/mutex.hpp"

namespace yocto 
{
	
	namespace threading
	{
		class thread 
		{	
		public:
#if defined(YOCTO_BSD)
			typedef pthread_t handle_t;
			typedef pthread_t id_t;
#endif
			
#if defined(YOCTO_WIN)
			typedef HANDLE handle_t;
			typedef DWORD  id_t;
#endif
			
			typedef void (*proc_t)( void *);
			
			explicit thread( thread::proc_t proc, void *data);
			void     join()   throw();
			virtual ~thread() throw(); //!< must be joined before !
			
			id_t     get_id() const throw();
			handle_t get_handle() const throw();
		
			static id_t     get_current_id() throw();
				
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(thread);
			proc_t       proc_;
			void        *data_;
#if defined(YOCTO_WIN)
			id_t         id32_;
#endif
			handle_t     handle_;

#if defined(YOCTO_BSD)
			static void * launch( void * ) throw();
#endif
			
#if defined(YOCTO_WIN)
			static DWORD WINAPI launch( LPVOID ) throw();
#endif
		};
	}
	
}

#endif
