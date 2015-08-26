//! \file

#ifndef YOCTO_THREADING_CONDITION_INCLUDED
#define YOCTO_THREADING_CONDITION_INCLUDED 1


#include "yocto/threading/mutex.hpp"

namespace yocto
{
#if defined(YOCTO_WIN)
	namespace win32
	{	
		class semaphore
		{
		public:
			semaphore() throw();
			~semaphore() throw();
			void wait() throw();
			void post() throw();
		private:
			HANDLE sem_;
			YOCTO_DISABLE_COPY_AND_ASSIGN(semaphore);
		};
	}
#endif
	namespace threading
	{
		
		//! portable condition variable
		class condition 
		{
		public:
			explicit condition() throw();
			virtual ~condition() throw();
			
			//! wait on a LOCKED mutex, returns on  the LOCKED mutex
			void     wait( mutex & ) throw();
			
			//! allow one blocked thread to wake up
			void     signal()    throw();
			
			//! allow to blocked thread to wake up
			void     broadcast() throw();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(condition);
            
#if defined(YOCTO_BSD)
			pthread_cond_t cond; //!< using native condition
#endif

#if defined(YOCTO_WIN)
			mutex            cv_lock;      /*!< condition lock               */
			int              cv_waiting;   /*!< waiting count                */
			int              cv_signals;   /*!< signals count                */
			win32::semaphore cv_wait_sem;  /*!< Win32 semaphore when waiting */
			win32::semaphore cv_done_sem;  /*!< Win32 semaphore when done    */
#endif

		};
	}

}

#endif
