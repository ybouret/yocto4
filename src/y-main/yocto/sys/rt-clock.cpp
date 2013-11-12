#include "yocto/sys/rt-clock.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/code/cast.hpp"

#if defined(YOCTO_APPLE)
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <iostream>
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
    
    rt_clock:: rt_clock() : data()
    {
        for(unsigned i=0;i<sizeof(data)/sizeof(data[0]);++i) data[i] = 0;
        calibrate();
    }
    
    rt_clock:: ~rt_clock() throw()
    {
        
    }
}

