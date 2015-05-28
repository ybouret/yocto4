#ifndef YOCTO_NET_IO_QUEUE_INCLUDED
#define YOCTO_NET_IO_QUEUE_INCLUDED 1

#include "yocto/net/io/cache.hpp"
#include "yocto/core/list.hpp"

#include "yocto/ios/ochannel.hpp"
#include "yocto/ios/ichannel.hpp"

namespace yocto
{
    namespace network
    {

        class io_queue : public ios::ochannel, public ios::ichannel
        {
        public:
            io_cache &shblk;

            io_queue( io_cache &shared ) throw();
            ~io_queue() throw();

            size_t recv(io_socket &iosock); //!< load data
            size_t send(io_socket &iosock); //!< emit data
            size_t bytes() const throw();
            size_t count() const throw();
            void   free() throw();

            virtual void put( const void *data, size_t size, size_t &done );
            virtual void get( void       *data, size_t size, size_t &done );

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(io_queue);
            core::list_of_cpp<io_block> blocks; //!< dynamic blocks
            size_t                      active; //!< total bytes, book keeping
            void see_before_writing();
            void burn_after_reading() throw();  //!< keep at most one empty block
        };

    }

}

#endif

