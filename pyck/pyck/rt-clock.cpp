#include "./rt-clock.hpp"
#include "./exception.hpp"
#include <cstring>

#if defined(PYCK_APPLE)
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif

#if defined(PYCK_LINUX) || defined(PYCK_FREEBSD) || defined(PYCK_SUNOS) || defined(PYCK_OPENBSD)
#define PYCK_USE_CLOCK_GETTIME 1
#endif

#if defined(PYCK_USE_CLOCK_GETTIME)
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#endif



#if defined(PYCK_APPLE)

void RunTimeClock::calibrate()
{
    mach_timebase_info_data_t timebase;
    const kern_return_t err = mach_timebase_info(&timebase);
    if(err != KERN_SUCCESS)
    {
        throw Exception("mach_timebase_info:%s",mach_error_string(err));
    }
    double conversion_factor = double(timebase.numer) / timebase.denom;
    freq = 1e-9 * conversion_factor;
}

uint64_t RunTimeClock:: Ticks()
{
    return mach_absolute_time();
}

double   RunTimeClock:: operator()(uint64_t num_ticks) const
{
    return freq * double(num_ticks);
}

#endif

#if defined(PYCK_USE_CLOCK_GETTIME)

static const uint64_t __giga64 = PYCK_U64(0x3B9ACA00);

void RunTimeClock:: calibrate()
{
    struct timespec tp  = { 0, 0 };
    const int       err = clock_getres( CLOCK_REALTIME, &tp );
    if(err!=0)
        throw Exception( errno, "clock_getres" );
    const uint64_t res = __giga64*uint64_t(tp.tv_sec) + uint64_t(tp.tv_nsec);
    freq = res;
}

uint64_t RunTimeClock:: Ticks()
{
    struct timespec tp  = { 0, 0 };
    const int       err = clock_gettime( CLOCK_REALTIME, &tp );
    if(err!=0)
        throw Exception( errno, "clock_gettime" );
    
    return __giga64*uint64_t(tp.tv_sec) + uint64_t(tp.tv_nsec);
}

double RunTimeClock:: operator()( uint64_t num_ticks ) const
{
    return 1e-9 * double(num_ticks);
}
#endif


#if defined(PYCK_WIN)
#include <windows.h>

void RunTimeClock:: calibrate()
{
    static const long double l_one = 1;
    LARGE_INTEGER F;
    if( ! :: QueryPerformanceFrequency( &F ) )
    {
        throw Exception( ::GetLastError(), "::QueryPerformanceFrequency" );
    }
    freq = l_one / static_cast<long double>( F.QuadPart );
}

uint64_t RunTimeClock:: Ticks()
{
    int64_t Q = 0;
    if( ! ::QueryPerformanceCounter( (LARGE_INTEGER *)&Q)  )
    {
        throw Exception( ::GetLastError(), " ::QueryPerformanceCounter" );
    }
    return uint64_t(Q);
}

double RunTimeClock:: operator()( uint64_t num_clicks ) const
{
    return freq * double(num_clicks);
}
#endif

RunTimeClock:: RunTimeClock() : freq(0)
{
    calibrate();
}

RunTimeClock:: ~RunTimeClock() throw()
{
    
}

