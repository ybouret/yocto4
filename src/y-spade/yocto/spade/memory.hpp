#ifndef YOCTO_SPADE_MEMORY_INCLUDED
#define YOCTO_SPADE_MEMORY_INCLUDED 1

#include "yocto/memory/global.hpp"

namespace yocto
{
    namespace spade
    {
        
        typedef memory::global memory_allocator;
        template <typename T> inline
        T *  memory_acquire( size_t &n ) { return memory::kind<memory_allocator>::acquire_as<T>(n); }
        
        template <typename T> inline
        void memory_release( T * &p, size_t &n ) throw() { memory::kind<memory_allocator>::release_as<T>(p,n); }
        
    }
}

#endif
