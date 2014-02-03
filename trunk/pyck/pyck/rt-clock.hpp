#ifndef YOCTO_SYS_RT_CLOCK_INCLUDED
#define YOCTO_SYS_RT_CLOCK_INCLUDED 1

#include "./os.hpp"

class RunTimeClock
{
public:
    explicit RunTimeClock();
    virtual ~RunTimeClock() throw();
    
    static uint64_t Ticks();     //!< system ticks
    void            calibrate(); //!< called during constructor, needed to transforms ticks to seconds.
    double          operator()( uint64_t num_ticks) const;
    
private:
    RunTimeClock(const RunTimeClock &);
    RunTimeClock &operator=(const RunTimeClock &);

#if defined(PYCK_APPLE) || defined(PYCK_WIN)
    double freq;
#else
    uint64_t freq;
#endif
};


#endif

