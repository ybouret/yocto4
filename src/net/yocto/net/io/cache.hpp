#ifndef YOCTO_NET_IO_CACHE_INCLUDED
#define YOCTO_NET_IO_CACHE_INCLUDED 1

#include "yocto/net/io/block.hpp"
#include "yocto/core/pool.hpp"

namespace yocto
{
    namespace network
    {

        class io_cache
        {
        public:
            const size_t block_size;

            explicit io_cache( const size_t blk_size ) throw();
            virtual ~io_cache() throw();


            size_t count() const throw(); //!< #blocks in pool
            size_t bytes() const throw(); //!< #bytes in pool

            //! store a previously allocated blk
            void      store( io_block *blk ) throw();

            //! query a block, initialized
            io_block *query();

        private:
            core::pool_of_cpp<io_block> blocks;
            YOCTO_DISABLE_COPY_AND_ASSIGN(io_cache);
        };

    }

}

#endif
