#ifndef PYCK_MUTEX_INCLUDED
#define PYCK_MUTEX_INCLUDED 1

#include "./lockable.hpp"

#if defined(PYCK_BSD)
#	if !defined(__USE_UNIX98)
#		define __USE_UNIX98
#	endif
#	include <pthread.h>
#endif /* BSD */

#if defined(PYCK_WIN)
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#endif /* WIN */

//! recursive mutex
class Mutex : public Lockable
{
public:
	explicit Mutex() throw();
	virtual ~Mutex() throw();

	virtual void lock()   throw();
	virtual void unlock() throw();

	static Mutex Giant;

private:
	Mutex( const Mutex & );
	Mutex &operator=( const Mutex & );
#if	defined(PYCK_BSD)
	pthread_mutex_t mutex_;
#endif

#if	defined(PYCK_WIN)
	CRITICAL_SECTION mutex_;
#endif

	void clear() throw();

};

#define PYCK_GIANT_LOCK() PYCK_LOCK( Mutex::Giant )

#endif
