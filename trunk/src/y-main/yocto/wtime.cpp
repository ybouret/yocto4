#include "yocto/wtime.hpp"
#include "yocto/exceptions.hpp"

#if defined(YOCTO_WIN)
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#endif

#if defined(YOCTO_BSD)
#include <cerrno>
#include <sys/time.h>
#endif

namespace yocto 
{
	
	wtime:: ~wtime() throw()
	{
		
	}
	
	wtime:: wtime( ) throw():
#if defined(YOCTO_BSD)
	data()
#endif
#if defined(YOCTO_WIN)
	quad(0),
	freq(0)
#endif
	{
		
	}
	
#if defined(YOCTO_WIN)
	void __wtime_fetch( int64_t &Q ) 
	{
		if( ! ::QueryPerformanceCounter( (LARGE_INTEGER *) &Q )  )
		{
			throw win32::exception( ::GetLastError(), " ::QueryPerformanceCounter" );
		}
	}
#endif
	
#if defined(YOCTO_BSD)
	void __wtime_fetch( struct timeval *tv )
	{
		if( 0 != gettimeofday( tv, NULL) )
		{
			throw libc:: exception( errno, "gettimeofday" );
		}
	}
#endif
	
	
	void wtime:: start()
	{
#if defined(YOCTO_WIN)
		static const long double l_one = 1;
		quad = 0;
		freq = 0;
		LARGE_INTEGER F;
		if( ! :: QueryPerformanceFrequency( &F ) )
		{
			throw win32::exception( ::GetLastError(), "::QueryPerformanceFrequency" );
		}
		freq = l_one / static_cast<long double>( F.QuadPart );
		__wtime_fetch( quad );
#endif
		
#if defined(YOCTO_BSD)
		__wtime_fetch( (struct timeval *)data );
#endif
	}
	
	double wtime:: query() const
	{
#if defined(YOCTO_WIN)
		int64_t now = 0;
		__wtime_fetch(now);
		return static_cast<double>( freq * static_cast<long double>(now-quad) );
#endif
		
#if defined(YOCTO_BSD)
		const struct timeval *old = (const struct timeval *) data;
		struct timeval now;
		__wtime_fetch( &now );
		const long int sec_old  = old->tv_sec;
		const long int sec_now  = now.tv_sec;
		const long int usec_old = old->tv_usec;
		const long int usec_now = now.tv_usec;
		return static_cast<double>(sec_now-sec_old) + 1e-6 * static_cast<double>(usec_now-usec_old);
#endif
	}
	
	uint64_t wtime:: ticks() 
	{
#if defined(YOCTO_WIN)
		int64_t ans = 0;
		__wtime_fetch( ans );
		return ans;
#endif
		
#if defined(YOCTO_BSD)
		static const uint64_t mega = 1000000;
		struct timeval now;
		__wtime_fetch( &now );
		return static_cast<uint64_t>(now.tv_usec) + mega * static_cast<uint64_t>(now.tv_sec);
#endif
	}
	
	uint32_t ihash32(uint32_t);
	
	uint32_t wtime:: seed() 
	{
		union 
		{
			uint64_t  ticks;
			uint32_t dw[2];
		} ini = { wtime::ticks() };
#if defined(__INTEL_COMPILER)
#	pragma warning ( disable : 981 )
#endif
		uint32_t seed = ihash32( ini.dw[0] ) ^ ihash32( ini.dw[1] );
		return seed;
	}
	
}