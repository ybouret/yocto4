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

        void io_queue:: free() throw()
        {
            while(blocks.size>1)
            {
                shblk.store( blocks.pop_front() );
            }
            if(blocks.size>=1)
            {
                blocks.head->initialize();
            }
            active = 0;
        }


        void io_queue:: see_before_writing()
        {
            io_block *blk = blocks.tail;
            if(blk!=0)
            {
                if(blk->space()<=0)
                {
                    if(blk->shift()>0)
                    {
                        blk->defragment();
                        assert(blk->space()>0);
                    }
                    else
                    {
                        blocks.push_back( shblk.query() );
                    }
                }
                else
                {
                    // do nothing
                }
            }
            else
            {
                blocks.push_back( shblk.query() );

            }

        }

        size_t io_queue:: load(io_socket &iosock)
        {

            // find some place
            see_before_writing();

            // write the content at the end
            assert(NULL!=blocks.tail);
            assert(blocks.tail->space()>0);
            const size_t nw = blocks.tail->load(iosock);
            active += nw;
            return nw;
        }


        void io_queue:: burn_after_reading() throw()
        {
            if(blocks.head->bytes() <=0 && blocks.size>1 )
            {
                shblk.store( blocks.pop_front() );
            }
        }

        size_t io_queue:: emit(io_socket &iosock)
        {
            if(blocks.head)
            {
                const size_t nr = blocks.head->emit(iosock);
                burn_after_reading();
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

        size_t io_queue:: count() const throw()
        {
            return blocks.size;
        }


        void io_queue:: put( const void *data, size_t size, size_t &done )
        {
            assert(!(NULL==data&&size>0));
            done = 0;
            see_before_writing();
            assert(NULL!=blocks.tail);
            assert(blocks.tail->space()>0);
            done = blocks.tail->store(data,size);
            active += done;
        }


        void io_queue:: get( void *data, size_t size, size_t &done )
        {
            assert(!(NULL==data&&size>0));
            if(blocks.head)
            {
                const size_t nr = blocks.head->query(data,size);
                burn_after_reading();
                assert(nr<=active);
                active -= nr;
                done=nr;
            }
            else
            {
                done = 0;
            }
        }
    }

}

