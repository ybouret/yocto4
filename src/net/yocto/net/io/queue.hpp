#ifndef YOCTO_NET_IO_QUEUE_INCLUDED
#define YOCTO_NET_IO_QUEUE_INCLUDED 1

#include "yocto/net/io/cache.hpp"
#include "yocto/core/list.hpp"

namespace yocto
{
    namespace network
    {

        class io_queue
        {
        public:
            io_cache &shblk;

            io_queue( io_cache &shared ) throw();
            ~io_queue() throw();

            size_t load(io_socket &iosock); //!< load data
            size_t emit(io_socket &iosock); //!< emit data
            size_t bytes() const throw();


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(io_queue);
            core::list_of_cpp<io_block> blocks; //!< dynamic blocks
            size_t                      active; //!< total bytes, book keeping

        };

    }

}

#endif

