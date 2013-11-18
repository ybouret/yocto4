#ifndef YOCTO_CODE_IN_STACK_INCLUDED
#define YOCTO_CODE_IN_STACK_INCLUDED 1

#include "yocto/os.hpp"

namespace yocto
{
    
    struct __stack
    {
        static void   init() throw();
        static bool   owns( const void *addr ) throw();
        static size_t used() throw();
    };
    
    
}

#endif
