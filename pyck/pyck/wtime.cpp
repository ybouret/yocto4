#include "./wtime.hpp"
#include "./exception.hpp"
#include "./mutex.hpp"

#if defined(PYCK_BSD)
#include <cerrno>
#include <sys/time.h>
#endif

#if defined(PYCK_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

WallTime:: WallTime() throw() :
#if defined(PYCK_BSD)
data()
#endif
#if defined(PYCK_WIN)
	quad(0),
	freq(0)
#endif
{
}

WallTime:: ~WallTime() throw()
{
}

#if defined(PYCK_WIN)
static void __wtime_fetch( int64_t &Q )
{
	if( ! ::QueryPerformanceCounter( (LARGE_INTEGER *) &Q )  )
	{
		throw Exception( ::GetLastError(), "::QueryPerformanceCounter" );
	}
}
#endif

#if defined(PYCK_BSD)
static void __wtime_fetch( struct timeval *tv )
{
	if( 0 != gettimeofday( tv, NULL) )
	{
		throw Exception( errno, "gettimeofday" );
	}
}
#endif

void WallTime:: start()
{
#if defined(PYCK_WIN)
	static const long double l_one = 1;
	quad = 0;
	freq = 0;
	LARGE_INTEGER F;
	PYCK_GIANT_LOCK();
	if( ! :: QueryPerformanceFrequency( &F ) )
	{
		throw Exception(::GetLastError(), "::QueryPerformanceFrequency");
	}
	freq = l_one / static_cast<long double>( F.QuadPart );
	__wtime_fetch( quad );
#endif

#if defined(PYCK_BSD)
	__wtime_fetch( (struct timeval *)(void*)data );
#endif
}

double WallTime:: query() const
{
#if defined(PYCK_WIN)
	int64_t now = 0;
	__wtime_fetch(now);
	return static_cast<double>( freq * static_cast<long double>(now-quad) );
#endif

#if defined(PYCK_BSD)
	const struct timeval *old = (const struct timeval *)(void*)data;
	struct timeval now;
	__wtime_fetch( &now );
	const long int sec_old  = old->tv_sec;
	const long int sec_now  = now.tv_sec;
	const long int usec_old = old->tv_usec;
	const long int usec_now = now.tv_usec;
	return static_cast<double>(sec_now-sec_old) + 1e-6 * static_cast<double>(usec_now-usec_old);
#endif
}

uint64_t WallTime:: Ticks()
{
#if defined(PYCK_WIN)
	int64_t ans = 0;
	__wtime_fetch( ans );
	return ans;
#endif

#if defined(PYCK_BSD)
	static const uint64_t mega = 1000000;
	struct timeval now;
	__wtime_fetch( &now );
	return static_cast<uint64_t>(now.tv_usec) + mega * static_cast<uint64_t>(now.tv_sec);
#endif
}

uint32_t WallTime:: Seed()
{
	union
	{
		uint64_t  ticks;
		uint32_t  dw[2];
	} ini = { WallTime::Ticks() };

	uint32_t seed = IntegerHash32( ini.dw[0] ) ^ IntegerHash32( ini.dw[1] );
	return seed;
}



