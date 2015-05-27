#include "yocto/net/io/block.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace network
    {
        io_block:: ~io_block() throw()
        {
            memory::global::__free(buffer);
        }

        io_block:: io_block( const size_t block_size) :
        next(0),
        prev(0),
        buflen( max_of<size_t>(block_size,sizeof(ptrdiff_t)) ),
        buffer( (uint8_t *)memory::global::__calloc(buflen, 1) )
        {
        }

        
        
    }
}
