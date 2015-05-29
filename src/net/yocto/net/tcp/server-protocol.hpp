#ifndef YOCTO_NET_TCP_SERVER_PROTOCOL_INCLUDED
#define YOCTO_NET_TCP_SERVER_PROTOCOL_INCLUDED 1

#include "yocto/net/tcp/protocol.hpp"

namespace yocto
{
    namespace network
    {

        //! a server protocol
        class server_protocol : public tcp_protocol
        {
        public:
            explicit server_protocol(const socket_address &ipaddr, const int max_pending, const size_t block_size );
            virtual ~server_protocol() throw();

            void execute();
            void suspend() throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(server_protocol);

            tcp_server server;
            void check_conn();

        };

    }
}

#endif
