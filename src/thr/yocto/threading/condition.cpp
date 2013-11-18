#include "yocto/threading/condition.hpp"
#include "yocto/error.hpp"

#if defined(YOCTO_WIN)
#include <limits.h>
#endif

namespace yocto
{
	
	
	
#if defined(YOCTO_WIN)
	namespace win32
	{
		semaphore:: semaphore() throw() : sem_( NULL )
		{
			YOCTO_GIANT_LOCK();
			sem_ = ::CreateSemaphore( NULL, 0, LONG_MAX, 0 );
			if( ! sem_ )
				win32::critical_error( ::GetLastError(), "::CreateSemaphore()");
		}
		
		semaphore:: ~semaphore() throw()
		{
			assert( sem_ );
			::CloseHandle( sem_ );
			sem_ = NULL;
		}
		
		void semaphore:: wait() throw()
		{
			assert( sem_ != NULL );
			const DWORD res = WaitForSingleObject(sem_,           /* handle to semaphore  */
												  INFINITE);      /* let's wait          */
			if( res != WAIT_OBJECT_0 )
				win32::critical_error( ::GetLastError(), "WaitForSingleObject( SEMAPHORE )");
		}
		
		void semaphore:: post() throw()
		{
			assert( sem_ != NULL );
			if( ! ::ReleaseSemaphore( sem_, 1, NULL ) )
				win32::critical_error( ::GetLastError(), "::ReleaseSemaphore" );
		}
	}
#endif
	
	
	
	namespace threading
	{
		
		
#if defined(YOCTO_BSD)
		condition:: condition() throw() : cond()
		{
			const int res = pthread_cond_init( & cond, NULL );
			if( res != 0 )
				libc::critical_error( res, "pthread_cond_init" );
		}
		
		condition:: ~condition() throw()
		{
			const int res = pthread_cond_destroy( & cond );
			if( res != 0 )
				libc:: critical_error( res, "pthread_cond_destroy" );
			
		}
		
		void condition:: wait( mutex &m ) throw()
		{
			const int res = pthread_cond_wait( & cond, & m.mutex_ );
			if( res != 0 )
				libc::critical_error(res, "pthread_cond_wait" );
		}
		
		void condition:: signal() throw()
		{
			const int res = pthread_cond_signal( & cond );
			if( res != 0 )
				libc::critical_error(res, "pthread_cond_signal");
		}
		
		void condition:: broadcast() throw()
		{
			const int res = pthread_cond_broadcast( & cond );
			if( res != 0 )
				libc::critical_error(res, "pthread_cond_broadcast");
		}
#endif
		
#if defined(YOCTO_WIN)
		condition:: condition() throw() :
		cv_lock(NULL),
		cv_waiting(0),
		cv_signals(0),
		cv_wait_sem(),
		cv_done_sem()
		{
		}
		
		condition:: ~condition() throw()
		{
			
		}
		
		void condition:: wait( mutex &m ) throw()
		{
			/* Obtain the protection mutex, and increment the number of waiters.
			 This allows the signal mechanism to only perform a signal if there
			 are waiting threads.
			 */
			cv_lock.lock();
			++cv_waiting;
			cv_lock.unlock();
			
			/* Unlock the mutex, as is required by condition variable semantics */
			m.unlock();
			
			
			/* Wait for a signal */
			cv_wait_sem.wait();
			
			
			/* Let the signaler know we have completed the wait, otherwise
			 the signaler can race ahead and get the condition semaphore
			 if we are stopped between the mutex unlock and semaphore wait,
			 giving a deadlock.  See the following URL for details:
			 http://www-classic.be.com/aboutbe/benewsletter/volume_III/Issue40.html
			 */
			
			cv_lock.lock();
			if( cv_signals > 0 )
			{
				/* We always notify the signal thread that we are done */
				cv_done_sem.post();
				--cv_signals;
			}
			--cv_waiting;
			cv_lock.unlock();
			
			/* Lock the mutex, as is required by condition variable semantics */
			m.lock();
		}
		
		void condition:: signal() throw()
		{
			cv_lock.lock();
			if( cv_waiting > cv_signals )
			{
				++cv_signals;
				cv_wait_sem.post();
				cv_lock.unlock();
				cv_done_sem.wait();
			}
			else
				cv_lock.unlock();
		}
		
		void condition:: broadcast() throw()
		{
			/* If there are waiting threads not already signalled, then
			 signal the condition and wait for the thread to respond.
			 */
			cv_lock.lock();
			if( cv_waiting > cv_signals )
			{
				const int num_waiting = cv_waiting - cv_signals;
				cv_signals = cv_waiting;
				for( int i=0; i < num_waiting; ++i )
				{
					cv_wait_sem.post();
				}
				/* Now all released threads are blocked here, waiting for us.
				 Collect them all.
				 */
				cv_lock.unlock();
				for( int i=0; i < num_waiting; ++i )
				{
					cv_done_sem.wait();
				}
			}
			else
			{
				cv_lock.unlock();
			}
		}
		
#endif
		
		
	}
	
}
