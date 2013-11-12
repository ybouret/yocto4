#include "yocto/sys/rt-clock.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/code/cast.hpp"
#include "yocto/threading/mutex.hpp"

#if defined(YOCTO_APPLE)
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif

#if defined(YOCTO_LINUX) || defined(YOCTO_FREEBSD)
#define YOCTO_USE_CLOCK_GETTIME 1
#endif

#if defined(YOCTO_USE_CLOCK_GETTIME)
#include <sys/time.h>
#include <errno.h>
#endif


#if defined(YOCTO_WIN)
#include "yocto/exceptions.hpp"
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
        *(double *)data = 1e-9 * conversion_factor;
    }
    
    uint64_t rt_clock:: ticks()
    {
		YOCTO_GIANT_LOCK();
        return mach_absolute_time();
    }
    
    double   rt_clock:: operator()( uint64_t num_ticks) const
    {
        const double factor = *(const double *)data;
        return factor * double(num_ticks);
    }

#endif
    
#if defined(YOCTO_CLOCK_GETTIME)
	static const uint64_t __giga64 = YOCTO_U64(0x3B9ACA00);

	void rt_clock:: calibrate()
	{
		YOCTO_GIANT_LOCK();
		struct timespec tp  = { 0, 0 };
		const int       err = clock_getres( CLOCK_REALTIME, &tp );
		if(err!=0)
			throw libc::exception( errno, "clock_getres" );
		const uint64_t res = uint64_t(tp.tv_sec) + __giga64 * uint64_t(tp.tv_nsec);
		*(uint64_t*)data = res;
	}
	
	uint64_t rt_clock:: ticks()
	{
		YOCTO_GIANT_LOCK();
		struct timespec tp  = { 0, 0 };
		const int       err = clock_gettime( CLOCK_REALTIME, &tp );
		if(err!=0)
			throw libc::exception( errno, "clock_gettime" );
		return uint64_t(tp.tv_sec) + __giga64 * uint64_t(tp.tv_nsec); 
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
		const double freq = l_one / static_cast<long double>( F.QuadPart );
		*(double *)data = freq;
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
		const double freq = *(const double *)data;
		return freq * double(num_clicks);
	}
#endif

#if defined(_MSC_VER)
#pragma warning ( disable : 4351 )
#endif

    rt_clock:: rt_clock() : data()
    {
        for(unsigned i=0;i<sizeof(data)/sizeof(data[0]);++i) data[i] = 0;
        calibrate();
    }
    
    rt_clock:: ~rt_clock() throw()
    {
        
    }
}

