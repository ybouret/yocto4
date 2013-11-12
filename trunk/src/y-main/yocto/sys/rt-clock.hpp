#ifndef YOCTO_SYS_RT_CLOCK_INCLUDED
#define YOCTO_SYS_RT_CLOCK_INCLUDED 1

#include "yocto/os.hpp"

namespace yocto
{
    class rt_clock
    {
    public:
        explicit rt_clock();
        virtual ~rt_clock() throw();
        
        uint64_t ticks();
        void     calibrate(); //!< called during constructor
        double   operator()( uint64_t num_ticks) const;
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(rt_clock);
        int64_t data[4];
        
    };
    
}

#endif

