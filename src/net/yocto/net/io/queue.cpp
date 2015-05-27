#include "yocto/net/io/queue.hpp"

namespace yocto
{
    namespace network
    {

        io_queue:: io_queue( io_cache &shared ) throw() :
        shblk(shared),
        blocks(),
        active(0)
        {
        }

        io_queue:: ~io_queue() throw()
        {
            while(blocks.size)
            {
                shblk.store( blocks.pop_front() );
            }
            active = 0;
        }


        size_t io_queue:: load(io_socket &iosock)
        {
            // find some place
            if(0==blocks.tail || 0 == blocks.tail->space())
            {
                blocks.push_back( shblk.query() );
            }
            // write the content at the end
            assert(NULL!=blocks.tail);
            assert(blocks.tail->space()>0);
            const size_t nw = blocks.tail->load(iosock);
            active += nw;
            return nw;
        }

        size_t io_queue:: emit(io_socket &iosock)
        {
            if(blocks.head)
            {
                const size_t nr = blocks.head->emit(iosock);
                if(blocks.head->bytes()<=0)
                {
                    shblk.store( blocks.pop_front() );
                }
                assert(nr<=active);
                active -= nr;
                return nr;
            }
            else
            {
                assert(active<=0);
                return 0;
            }
        }

        size_t io_queue:: bytes() const throw()
        {
            return active;
        }
    }

}

