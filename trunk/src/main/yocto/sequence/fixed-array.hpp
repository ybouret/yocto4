#ifndef YOCTO_FIXED_ARRAY_INCLUDED
#define YOCTO_FIXED_ARRAY_INCLUDED 1

#include "yocto/sequence/array.hpp"

namespace yocto
{
    
    template <
    typename T,
    typename ALLOCATOR = memory::global::allocator>
    class fixed_array : public array<T>
    {
    public:
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(array)
    };
}

#endif
