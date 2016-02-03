#include "yocto/mpk/types.hpp"
#include "yocto/exception.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace mpk
    {

        const char allocator::name[] = "mpk";
        
        allocator:: ~allocator() throw()
        {
        }

        allocator::allocator() throw() :
        singleton<allocator>(),
        blocks(max_bytes)
        {

        }
        
        size_t allocator::align(size_t n)
        {
            
        }

    }
}

