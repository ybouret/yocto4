#include "yocto/sys/rt-clock.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/threading/mutex.hpp"
#include <cstring>

#if defined(YOCTO_APPLE)
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif

#if defined(YOCTO_LINUX) || defined(YOCTO_FREEBSD) || defined(YOCTO_SUNOS) || defined(YOCTO_OPENBSD)
#define YOCTO_USE_CLOCK_GETTIME 1
#endif

#if defined(YOCTO_USE_CLOCK_GETTIME)
#include <sys/time.h>
#include <errno.h>
#include <iostream>
#endif

namespace yocto
{

#if defined(YOCTO_APPLE)

	void rt_clock::calibrate()
	{
		YOCTO_GIANT_LOCK();
		mach_timebase_info_data_t timebase;
		const kern_return_t err = mach_timebase_info(&timebase);
		if(err != KERN_SUCCESS)
		{
			throw imported::exception("mach_timebase_info","%s",mach_error_string(err));
		}
		double conversion_factor = double(timebase.numer) / timebase.denom;
		freq = 1e-9 * conversion_factor;
	}

	uint64_t rt_clock:: ticks()
	{
		YOCTO_GIANT_LOCK();
		return mach_absolute_time();
	}

	double   rt_clock:: operator()(uint64_t num_ticks) const
	{
		return freq * double(num_ticks);
	}

#endif

#if defined(YOCTO_USE_CLOCK_GETTIME)

	static const uint64_t __giga64 = YOCTO_U64(0x3B9ACA00);

	void rt_clock:: calibrate()
	{
		YOCTO_GIANT_LOCK();
		struct timespec tp  = { 0, 0 };
		const int       err = clock_getres( CLOCK_REALTIME, &tp );
		if(err!=0)
			throw libc::exception( errno, "clock_getres" );
		const uint64_t res = __giga64*uint64_t(tp.tv_sec) + uint64_t(tp.tv_nsec); 
		freq = res;
	}

	uint64_t rt_clock:: ticks()
	{
		YOCTO_GIANT_LOCK();
		struct timespec tp  = { 0, 0 };
		const int       err = clock_gettime( CLOCK_REALTIME, &tp );
		if(err!=0)
			throw libc::exception( errno, "clock_gettime" );
		
		return __giga64*uint64_t(tp.tv_sec) + uint64_t(tp.tv_nsec); 
	}

	double rt_clock:: operator()( uint64_t num_ticks ) const
	{
		return 1e-9 * double(num_ticks);
	}
#endif


#if defined(YOCTO_WIN)
	void rt_clock:: calibrate()
	{
		static const long double l_one = 1;
		YOCTO_GIANT_LOCK();
		LARGE_INTEGER F;
		if( ! :: QueryPerformanceFrequency( &F ) )
		{
			throw win32::exception( ::GetLastError(), "::QueryPerformanceFrequency" );
		}
        freq = l_one / static_cast<long double>( F.QuadPart );
	}

	uint64_t rt_clock:: ticks() 
	{
		YOCTO_GIANT_LOCK();
		int64_t Q = 0;
		if( ! ::QueryPerformanceCounter( (LARGE_INTEGER *)&Q)  )
		{
			throw win32::exception( ::GetLastError(), " ::QueryPerformanceCounter" );
		}
		return uint64_t(Q);
	}

	double rt_clock:: operator()( uint64_t num_clicks ) const 
	{
		return freq * double(num_clicks);
	}
#endif

#if defined(_MSC_VER)
#pragma warning ( disable : 4351 )
#endif

	rt_clock:: rt_clock() : freq(0)
	{
		calibrate();
	}

	rt_clock:: ~rt_clock() throw()
	{

	}
}

