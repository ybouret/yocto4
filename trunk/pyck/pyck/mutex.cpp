#include "./mutex.hpp"
#include "./error.hpp"
#include <cstring>

void Mutex:: clear() throw()
{
    memset( &mutex_,0,sizeof(mutex_));
}

Mutex:: ~Mutex() throw()
{
    
#if defined(PYCK_WIN)
    ::DeleteCriticalSection( & mutex_ );
#endif
    
#if defined(PYCK_BSD)
    {
        const int res = pthread_mutex_destroy( &mutex_ );
        if( res != 0 )
            LibcCriticalError( res, "pthread_mutex_destroy" );
    }
#endif
    
    clear();
}


Mutex:: Mutex() throw() :
mutex_()
{
    clear();
    
    //-- initialize the mutex
#if defined(PYCK_WIN)
    ::InitializeCriticalSection( & mutex_ );
#endif
    
#if defined(PYCK_BSD)
    pthread_mutexattr_t attr;
    
    int  res = pthread_mutexattr_init( &attr );
    if (res != 0)
        LibcCriticalError(res, "pthread_mutexattr_init");
    
    res = pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE);
    if (res != 0)
    {
        pthread_mutexattr_destroy( &attr );
        LibcCriticalError(res, "pthread_mutexattr_settype(RECURSIVE)");
    }
    
    res = pthread_mutex_init( & mutex_ , &attr );
    (void) pthread_mutexattr_destroy( &attr );
    if( res != 0 )
        LibcCriticalError(res,"pthread_mutex_init");
#endif
    
}

void Mutex::lock() throw()
{
    
#if defined(PYCK_WIN)
    ::EnterCriticalSection(  & mutex_  );
#endif
    
#if defined(PYCK_BSD)
    const int res = pthread_mutex_lock( &mutex_ );
    if( res != 0 )
        LibcCriticalError( res,  "pthread_mutex_lock" );
#endif
    
}


void Mutex:: unlock() throw()
{
    
#if defined(PYCK_WIN)
    ::LeaveCriticalSection( &mutex_  );
#endif
    
#if defined(PYCK_BSD)
    const int res = pthread_mutex_unlock( & mutex_ );
    if( res != 0 )
        LibcCriticalError( res,  "pthread_mutex_lock" );
#endif
}



