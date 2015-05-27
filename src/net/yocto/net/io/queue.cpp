#include "yocto/net/io/queue.hpp"

namespace yocto
{
    namespace network
    {

        io_queue:: io_queue( io_cache &shared ) throw() :
        shblk(shared),
        blocks()
        {
        }

        io_queue:: ~io_queue() throw()
        {
            while(blocks.size)
            {
                shblk.store( blocks.pop_front() );
            }
        }

    }

}

