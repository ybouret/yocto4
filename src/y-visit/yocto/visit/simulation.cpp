#include "yocto/visit/interface.hpp"
#include "yocto/exception.hpp"

namespace yocto 
{
    visit:: simulation:: ~simulation() throw()
    {
    }
        
    visit:: simulation:: simulation() throw() :
    cycle(0),
    runMode(VISIT_SIMMODE_STOPPED),
    done(false)
    {
    }
    
    
    
}
