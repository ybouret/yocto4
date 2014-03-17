//! \file

#ifndef YOCTO_MPA_TYPES_INCLUDED
#define YOCTO_MPA_TYPES_INCLUDED 1

#include "yocto/os.hpp"

namespace yocto
{
    
    namespace mpa
    {
        class    memmgr;
        struct   memIO
        {
            static uint8_t *acquire(size_t &n);
            static void     release(uint8_t * &p, size_t &n) throw();
        };
        
    }
    
}

#endif
