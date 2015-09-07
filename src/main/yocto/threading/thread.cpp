#include "yocto/threading/thread.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/error.hpp"
#include "yocto/object.hpp"
#include <cstring>

namespace yocto
{
	namespace  threading
	{
        

        //======================================================================
        //
        // thread ctor
        //
        //======================================================================
        thread:: thread( mutex &shared_access ) throw() :
        access(shared_access),
        handle(),
        identifier(),
        proc(0),
        data(0),
        code(0),
        stop(true),
        next(0),
        prev(0)
        {
        }

        vslot &thread:: check_code()
        {
            if(!code.is_valid()) code.reset( new vslot() );
            return *code;
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
            memset( &handle,     0, sizeof(handle) );
            memset( &identifier, 0, sizeof(identifier));
            memset( &proc,       0, sizeof(proc)   );
            memset( &data,       0, sizeof(data)   );
            if(code.is_valid())  code->free();
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
            identifier = handle;
            //------------------------------------------------------------------
            // </pthread>
            //------------------------------------------------------------------
#endif
			
#if defined(YOCTO_WIN)
            //------------------------------------------------------------------
            // <win32>
            //------------------------------------------------------------------
            YOCTO_GIANT_LOCK();
			handle = ::CreateThread(0 ,
                                    0 ,
                                    thread::entry,
                                    this,
                                    0,
                                    & identifier );
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

		 //======================================================================
        //
        // get the current thread identifier
        //
        //======================================================================
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
    
    
}
