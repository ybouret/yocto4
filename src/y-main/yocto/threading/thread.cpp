#include "yocto/threading/thread.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/error.hpp"

#include <cstring>
#include <new>

namespace yocto 
{
	namespace  threading 
	{
		
		
		
#if defined(YOCTO_WIN)
		DWORD WINAPI thread::launch( LPVOID args ) throw()
		{
			thread *thr= static_cast<thread *>(args);
			assert( thr );
			thr->proc_( thr->data_ );		
			return 0;
		}
		
#endif
		
#if defined(YOCTO_BSD)
		void * thread::launch( void *args ) throw()
		{
			thread *thr= static_cast<thread *>(args);
			assert( thr );
			thr->proc_( thr->data_ );
			return NULL;
		}
#endif
		
		
		
		
		thread:: thread( thread::proc_t proc, void *data ) :
		proc_( proc ),
		data_( data ),
#if defined(YOCTO_WIN)
		id32_(0),
#endif
		handle_()
		{
#if	defined(YOCTO_BSD)
			const int res = pthread_create( &handle_, NULL, thread::launch, this);
			if( res != 0 )
				throw libc::exception( res, "pthread_create" );
#endif
			
#if defined(YOCTO_WIN)
			YOCTO_GIANT_LOCK();
			handle_ = ::CreateThread(0 ,
									 0 ,
									 thread::launch,
									 this,
									 0,
									 & id32_ );
			if( NULL == handle_ )
			{
				const DWORD res = ::GetLastError();
				throw win32::exception( res, "CreateThread" );
			}
			
#endif
		}
		
		
		void thread:: join() throw()
		{
#if defined(YOCTO_BSD)
			const int res = pthread_join( handle_, 0 );
			if( res != 0 )
			{
				libc::critical_error( res, "pthread_join" );
			}
#endif
			
#if defined(YOCTO_WIN)
			YOCTO_GIANT_LOCK();
			if( ::WaitForSingleObject( handle_ , INFINITE ) != WAIT_OBJECT_0 )
			{
				win32::critical_error( ::GetLastError(), "WaitForSingleObject" );
			}
#endif
		}
		
		
		thread:: ~thread() throw()
		{
#if defined(YOCTO_BSD)
			
#endif
			
#if defined(YOCTO_WIN)
			::CloseHandle( handle_ );
#endif
		}
		
		thread::id_t thread::get_id() const throw()
		{
#if defined(YOCTO_BSD)
			return handle_;
#endif
			
#if defined(YOCTO_WIN)
			return id32_;
#endif
		}
		
		thread::handle_t thread:: get_handle() const throw()
		{
			return handle_;
		}
		
		thread::handle_t thread:: get_current_handle() throw()
		{
#if defined(YOCTO_BSD)
			return pthread_self();
#endif
			
#if defined(YOCTO_WIN)
			return ::GetCurrentThread();
#endif
			
		}
		
		thread::id_t thread:: get_current_id() throw()
		{
#if defined(YOCTO_BSD)
			return pthread_self();
#endif
			
#if defined(YOCTO_WIN)
			return ::GetCurrentThreadId();
#endif
		}
        
	}
    
    void thread_proxy:: clear() throw()
    {
        memset( block, 0, sizeof(block) );
    }
    
    thread_proxy:: thread_proxy() throw() :
    block()
    {
        clear();
    }
    
    thread_proxy:: ~thread_proxy() throw()
    {
        clear();
    }
    
    void thread_proxy:: launch( threading::thread::proc_t proc, void *data )
    {
        try
        {
            new ( &block[0] ) threading::thread(proc,data);
        }
        catch(...)
        {
            clear();
            throw;
        }
    }
    
    void thread_proxy:: finish() throw()
    {
        static_cast<threading::thread *>( (void*)&block[0] )->join();
        clear();
    }

}
