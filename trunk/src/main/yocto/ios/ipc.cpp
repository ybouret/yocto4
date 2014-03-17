#include "yocto/ios/ipc.hpp"
#include "yocto/exceptions.hpp"

#if defined(YOCTO_BSD)
#include <unistd.h>
#include <cerrno>
#endif

namespace yocto 
{
    
    namespace ios 
    {
        
#if 0
#if defined(YOCTO_BSD)
        namespace
        {
            static const size_t FD_READ  = 0;
            static const size_t FD_WRITE = 1;
            static const size_t FD_COUNT = 2;
        }            
#endif
#endif
        
    }
    
}
