#include "yocto/net/io/cache.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace network
    {
        io_cache:: io_cache( const size_t blk_size ) throw() :
        block_size( max_of<size_t>(blk_size,sizeof(ptrdiff_t))),
	blocks()
        {
        }

        io_cache:: ~io_cache() throw()
        {
            
        }

        size_t io_cache:: count() const throw() { return blocks.size; }
        size_t io_cache:: bytes() const throw() { return blocks.size * block_size; }


        void io_cache:: store(io_block *blk) throw()
        {
            assert(blk);
            assert(block_size==blk->len);
            blocks.store(blk);
        }

        io_block * io_cache:: query()
        {
            if( blocks.size )
            {
                io_block *blk = blocks.query();
                blk->initialize();
                return blk;
            }
            else
            {
                return new io_block(block_size);
            }
        }
        
    }
}
