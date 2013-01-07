#include "./thread.hpp"
#include "./exception.hpp"
#include "./error.hpp"

#if defined(PYCK_WIN)
DWORD WINAPI Thread:: Launch( LPVOID args ) throw()
{
    Thread *thr= static_cast<Thread *>(args);
    assert( thr );
    thr->proc_( thr->data_ );
    return 0;
}

#endif

#if defined(PYCK_BSD)
void * Thread:: Launch( void *args ) throw()
{
    Thread *thr= static_cast<Thread *>(args);
    assert( thr );
    thr->proc_( thr->data_ );
    return NULL;
}
#endif



Thread:: Thread( Thread::Proc proc, void *data) :
proc_( proc ),
data_( data ),
#if defined(PYCK_WIN)
id32_(0),
#endif
handle_()
{
#if	defined(PYCK_BSD)
    const int res = pthread_create( &handle_, NULL, Thread:: Launch, this);
    if( res != 0 )
        throw Exception( res, "pthread_create" );
#endif
    
#if defined(PYCK_WIN)
    handle_ = ::CreateThread(0 ,
                             0 ,
                             Thread::Launch,
                             this,
                             0,
                             & id32_ );
    if( NULL == handle_ )
    {
        throw Exception( ::GetLastError(), "::CreateThread" );
    }
#endif
}


void Thread:: join() throw()
{
#if defined(PYCK_BSD)
    const int res = pthread_join( handle_, 0 );
    if( res != 0 )
    {
        SystemCriticalError( res, "pthread_join" );
    }
#endif
    
#if defined(PYCK_WIN)
    if( ::WaitForSingleObject( handle_ , INFINITE ) != WAIT_OBJECT_0 )
    {
        SystemCriticalError( ::GetLastError(), "WaitForSingleObject" );
    }
#endif
}

Thread:: ~Thread() throw()
{
#if defined(PYCK_WIN)
    ::CloseHandle( handle_ );
#endif
}
