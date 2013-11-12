#include "yocto/sys/rt-clock.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/code/cast.hpp"

#if defined(YOCTO_APPLE)
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <iostream>
#endif

#if defined(YOCTO_LINUX)
#include <sys/time.h>
#include <errno.h>
#include "yocto/threading/mutex.hpp"
#endif

namespace yocto
{

#if defined(YOCTO_APPLE)
  
    void rt_clock::calibrate()
    {
        std::cerr << "sizeof(...)=" << sizeof(mach_timebase_info_data_t) << std::endl;
        mach_timebase_info_data_t timebase;
        const kern_return_t err = mach_timebase_info(&timebase);
        if(err != KERN_SUCCESS)
        {
            throw imported::exception("mach_timebase_info","%s",mach_error_string(err));
        }
        double conversion_factor = double(timebase.numer) / timebase.denom;
        std::cerr << "conversion_factor=" << conversion_factor << std::endl;
        *(double *)data = 1e-9 * conversion_factor;
    }
    
    uint64_t rt_clock:: ticks()
    {
        return mach_absolute_time();
    }
    
    double   rt_clock:: operator()( uint64_t num_ticks) const
    {
        const double factor = *(const double *)data;
        return factor * double(num_ticks);
    }

#endif
    
#if defined(YOCTO_LINUX)
	void rt_clock:: calibrate()
	{
		YOCTO_GIANT_LOCK();
		struct timespec tp  = { 0, 0 };
		const int       err = clock_getres( CLOCK_REALTIME, &tp );
		if(err!=0)
			throw libc::exception( errno, "clock_gettime" );
		
	}
	
	uint64_t rt_clock:: ticks()
	{
		static const uint64_t giga = YOCTO_U64(0x3B9ACA00);
		YOCTO_GIANT_LOCK();
		struct timespec tp  = { 0, 0 };
		const int       err = clock_gettime( CLOCK_REALTIME, &tp );
		if(err!=0)
			throw libc::exception( errno, "clock_gettime" );
		return uint64_t(tp.tv_sec) + giga * uint64_t(tp.tv_nsec); 
	}
	
	double rt_clock:: operator()( uint64_t num_ticks ) const
	{
		return 1e-9 * double(num_ticks);
	}
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

