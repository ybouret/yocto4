#include "yocto/threading/thread.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/error.hpp"
#include "yocto/object.hpp"
#include <cstring>

namespace yocto
{
	namespace  threading
	{
        
#if USE_OLD_THREAD
        
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
        
        
#else
        
        //======================================================================
        //
        // thread ctor
        //
        //======================================================================
        thread:: thread( mutex &shared_access ) throw() :
        access(shared_access),
        handle(),
        proc(0),
        data(0),
        code(),
        stop(true),
        next(0),
        prev(0)
        {
        }
        
        //======================================================================
        //
        // thread creation with a shared mutex
        //
        //======================================================================
        thread * thread:: create_with( mutex & access_ref )
        {
            return new (object::acquire1<thread>()) thread( access_ref );
        }
        
        //======================================================================
        //
        // release memory
        //
        //======================================================================
        thread:: ~thread() throw()
        {
            assert(0==next);
            assert(0==prev);
            
        }
        void thread:: destruct( thread *thr ) throw()
        {
            assert(thr);
            thr->~thread();
            object::release1<thread>(thr);
        }
        
        
        //======================================================================
        //
        // system entry point
        //
        //======================================================================
#if defined(YOCTO_WIN)
		DWORD WINAPI thread::entry( LPVOID args ) throw()
		{
			thread *thr= static_cast<thread *>(args);
			assert( thr );
			thr->proc( thr->data );
			return 0;
		}
		
#endif
		
#if defined(YOCTO_BSD)
		void * thread::entry( void *args ) throw()
		{
			thread *thr= static_cast<thread *>(args);
			assert( thr );
			thr->proc( thr->data );
			return NULL;
		}
#endif
		
        
        //======================================================================
        //
        // cleanup
        //
        //======================================================================
        void thread:: clear() throw()
        {
            YOCTO_LOCK(access);
            memset( &handle, 0, sizeof(handle) );
            memset( &proc,   0, sizeof(proc)   );
            memset( &data,   0, sizeof(data)   );
            code.free();
            (bool&)stop = true;
        }
        
        //======================================================================
        //
        // generic launch
        //
        //======================================================================
        void thread:: launch( procedure user_proc, void *user_data )
        {
            assert(user_proc);
            
            //__________________________________________________________________
            //
            // let us get the access
            //__________________________________________________________________
            YOCTO_LOCK(access);
            assert(0==proc);
            assert(0==data);
            
            //__________________________________________________________________
            //
            // record what to do for the thread
            //__________________________________________________________________
            proc = user_proc;
            data = user_data;
            
            //__________________________________________________________________
            //
            // system thread
            //__________________________________________________________________
            (bool &)stop = false;
            
            
#if	defined(YOCTO_BSD)
            //------------------------------------------------------------------
            // <pthread>
            //------------------------------------------------------------------
			const int res = pthread_create( &handle, NULL, thread::entry, this);
			if( res != 0 )
            {
                clear();
				throw libc::exception( res, "pthread_create" );
            }
            //------------------------------------------------------------------
            // </pthread>
            //------------------------------------------------------------------
#endif
			
#if defined(YOCTO_WIN)
            //------------------------------------------------------------------
            // <win32>
            //------------------------------------------------------------------
            YOCTO_GIANT_LOCK();
            DWORD id32 = 0;
			handle = ::CreateThread(0 ,
                                    0 ,
                                    thread::entry,
                                    this,
                                    0,
                                    & id32 );
			if( NULL == handle )
			{
				const DWORD res = ::GetLastError();
                clear();
				throw win32::exception( res, "CreateThread" );
			}
            //------------------------------------------------------------------
            // </win32>
            //------------------------------------------------------------------
#endif
            
            
        }
        
        //======================================================================
        //
        // finish a running thread
        //
        //======================================================================
        void thread:: finish() throw()
        {
            {
                YOCTO_LOCK(access);
                if(!proc)
                    return;
                (bool&)stop = true;
            }
            
#if defined(YOCTO_BSD)
			const int res = pthread_join( handle, 0 );
			if( res != 0 )
			{
                clear();
				libc::critical_error( res, "pthread_join" );
			}
#endif
			
#if defined(YOCTO_WIN)
			YOCTO_GIANT_LOCK();
			if( ::WaitForSingleObject( handle , INFINITE ) != WAIT_OBJECT_0 )
			{
                clear();
				win32::critical_error( ::GetLastError(), "WaitForSingleObject" );
			}
            ::CloseHandle( handle );
#endif
            
            clear();
        }
        
        //======================================================================
        //
        // get the system handle for a running thread
        //
        //======================================================================
        thread::handle_t thread:: get_handle() const throw()
        {
            YOCTO_LOCK(access);
            assert(proc!=0);
            return handle;
        }
        
        //======================================================================
        //
        // get the current thread handle
        //
        //======================================================================
        thread::handle_t thread:: get_current_handle() throw()
        {
            
#if defined(YOCTO_BSD)
            return pthread_self();
#endif
            
#if defined(YOCTO_WIN)
            return ::GetCurrentThread();
#endif
        }
        
#endif
      
#if 0
        //======================================================================
        //
        // C++
        //
        //======================================================================
        static inline void execute_code( void *args ) throw()
        {
            try
            {
                assert(args);
                thread::callback &cb = *static_cast<thread::callback *>(args);
                cb();
            }
            catch(...)
            {
            }
        }
        
        void thread::launch(const callback &cb)
        {
            //__________________________________________________________________
            //
            // let us get the access
            //__________________________________________________________________
            YOCTO_LOCK(access);
            assert(0==proc);
            assert(0==data);
            
            //__________________________________________________________________
            //
            // register the callback
            //__________________________________________________________________
            code.make<callback>(cb);
            
            //__________________________________________________________________
            //
            // low level
            //__________________________________________________________________
            launch(execute_code, & code.as<callback>() );
        }
        
        
        
        
        static inline
        void execute_proc( void *args )
        {
            try
            {
                assert(args);
                thread::c_proc fn = thread::c_proc(args);
                fn();
            }
            catch(...)
            {
                
            }
        }
        
        
        void thread:: launch( const c_proc fn )
        {
            //__________________________________________________________________
            //
            // let us get the access
            //__________________________________________________________________
            YOCTO_LOCK(access);
            assert(0==proc);
            assert(0==data);
            
            //__________________________________________________________________
            //
            // register the callback
            //__________________________________________________________________
            code.make<c_proc>(fn);
            
            //__________________________________________________________________
            //
            // low level
            //__________________________________________________________________
            launch(execute_proc, (void*)code.as<c_proc>() );

        }
#endif
        
	}
    
    
}
