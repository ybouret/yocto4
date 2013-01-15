#ifndef PYCK_THREAD_INCLUDED
#define PYCK_THREAD_INCLUDED 1

#include "./mutex.hpp"

class Thread
{
public:


#if defined(PYCK_BSD)
	typedef pthread_t Handle;
	typedef pthread_t ID;
#endif

#if defined(PYCK_WIN)
	typedef HANDLE Handle;
	typedef DWORD  ID;
#endif

	typedef void (*Proc)( void *);

	explicit Thread( Thread::Proc proc, void *data);
	virtual ~Thread() throw();

	void join() throw();


private:
	Thread( const Thread & );
	Thread &operator=( const Thread & );
	Proc         proc_;
	void        *data_;

#if defined(PYCK_WIN)
	static DWORD WINAPI Launch( LPVOID ) throw();
	ID           id32_;
#endif
	Handle       handle_;

#if defined(PYCK_BSD)
	static void * Launch( void * ) throw();
#endif


};

#endif
