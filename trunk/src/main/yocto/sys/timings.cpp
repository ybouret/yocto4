#include "yocto/sys/timings.hpp"

namespace yocto
{
    
    timings:: ~timings() throw() {}
    
    timings:: timings() :
    wtime(),
    cycle(0),
    run_t(0),
    speed(0),
    start(0)
    {
    }

    
}
