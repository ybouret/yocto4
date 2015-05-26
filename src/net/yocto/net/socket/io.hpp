#ifndef YOCTO_NET_SOCKET_IO_INCLUDED
#define YOCTO_NET_SOCKET_IO_INCLUDED 1

#include "yocto/net/socket.hpp"

namespace yocto
{
    namespace network
    {

        class socket_io
        {
        public:
            virtual ~socket_io() throw();
            
            virtual size_t send( const void *, size_t ) = 0; //!< TCP send or UDP sendto
            virtual size_t recv( void       *, size_t ) = 0; //!< TCP recv or UDP recvfrom

        protected:
            explicit socket_io() throw();

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(socket_io);
        };

    }
}

#endif

