#include "yocto/spade/data-block.hpp"
#include "yocto/spade/memory.hpp"

namespace yocto
{
    namespace spade
    {
        
        data_block:: ~data_block() throw()
        {
            memory::kind<memory_allocator>::release(data,size);
        }
        
        data_block:: data_block( linear &l ) :
        size( l.bytes ),
        data( memory::kind<memory_allocator>::acquire(size) )
        {
            l.link(data);
        }
        
    }
}

