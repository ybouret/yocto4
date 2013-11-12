#if defined(YOCTO_BSD)
#define _POSIX_C_SOURCE 199309 
#endif

#include "yocto/sys/wtime.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/code/utils.hpp"

#if defined(YOCTO_WIN)
#include "yocto/threading/mutex.hpp"
#endif

#if defined(YOCTO_BSD)
#include <cerrno>
#include <time.h>
#include <sys/time.h>
#include <cmath>
#endif

#include "yocto/sys/pid.hpp"

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
	static void __wtime_fetch( int64_t &Q ) 
	{
		if( ! ::QueryPerformanceCounter( (LARGE_INTEGER *) &Q )  )
		{
			throw win32::exception( ::GetLastError(), " ::QueryPerformanceCounter" );
		}
	}
#endif
	
#if defined(YOCTO_BSD)
	static void __wtime_fetch( struct timeval *tv )
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
		YOCTO_GIANT_LOCK();
		if( ! :: QueryPerformanceFrequency( &F ) )
		{
			throw win32::exception( ::GetLastError(), "::QueryPerformanceFrequency" );
		}
		freq = l_one / static_cast<long double>( F.QuadPart );
		__wtime_fetch( quad );
#endif
		
#if defined(YOCTO_BSD)
		__wtime_fetch( (struct timeval *)(void*)data );
#endif
	}
	
	double wtime:: query() const
	{
#if defined(YOCTO_WIN)
		int64_t now = 0;
		__wtime_fetch(now);
		return static_cast<double>( freq * static_cast<long double>(now-quad) );
#endif
        
#if defined(__ICC)
#pragma warning ( disable : 2259 ) 
#endif	
#if defined(YOCTO_BSD)
		const struct timeval &old = *(const struct timeval *)(void*)data;
		struct timeval now = { 0, 0 };
		__wtime_fetch( &now );
		const long int sec_old  = old.tv_sec;
		const long int sec_now  = now.tv_sec;
		const long int usec_old = old.tv_usec;
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

#if defined(__ICC)
#pragma warning ( disable : 1419 ) 
#endif
	
	extern uint32_t ihash32(uint32_t);
	
	uint32_t wtime:: seed()
	{
		union 
		{
			uint64_t  ticks;
			uint32_t  dw[2];
		} ini = { wtime::ticks() };
#if defined(__INTEL_COMPILER)
#	pragma warning ( disable : 981 )
#endif
        const uint32_t data[4] = { ini.dw[0], ini.dw[1], ihash32( ini.dw[0] ) ^ ihash32( ini.dw[1] ), get_process_h32()  };
        
		uint32_t seed = hash32(data, sizeof(data) );
		return seed;
	}
	
	void wtime:: sleep( double seconds ) throw()
	{
		static const double sleep_min = 0;     /*          */
		static const double sleep_max = 86400; /*  one day */
		
		double s = clamp<double>(sleep_min,seconds,sleep_max);
		
#if defined(YOCTO_BSD)
		struct timespec info = { 0, 0 };
		struct timespec left = { 0, 0 };
		double num_secs      = 0;
		double num_nano      = modf(s,&num_secs);
		num_nano             = floor( 1.0e9 * num_nano );
		info.tv_sec  = (unsigned int)num_secs;
		info.tv_nsec = (unsigned int)num_nano;
		while( info.tv_nsec >= 1000000000 ) {
			++info.tv_sec;
			info.tv_nsec -= 1000000000;
		}
		while( nanosleep( &info, &left) < 0 ) {
			assert( errno == EINTR );
			info = left;
		}
#endif
		
		
#if defined(YOCTO_WIN)
		::Sleep( (DWORD)(s * 1000.0) );
#endif
	}
	
}
