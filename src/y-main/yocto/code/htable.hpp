#ifndef YOCTO_CODE_HTABLE_INCLUDED
#define YOCTO_CODE_HTABLE_INCLUDED 1

#include "yocto/os.hpp"

namespace yocto
{
    
    struct htable
    {
        static const size_t load_factor = 4;
        static const size_t min_slots   = 3;
        
        //! return the adapted prime size
        static size_t compute_slots_for( size_t &items );
                
    };
    
}


#endif

