#ifndef YOCTO_NET_TCP_CLIENT_PROTOCOL_INCLUDED
#define YOCTO_NET_TCP_CLIENT_PROTOCOL_INCLUDED 1

#include "yocto/net/tcp/protocol.hpp"

namespace yocto
{
    namespace network
    {

        class client_protocol : public tcp_protocol
        {
        public:
            explicit client_protocol(const size_t block_size );
            virtual ~client_protocol() throw();

            bool dial( const socket_address &ipaddr );

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(client_protocol);
            bool running;

        };
        
    }
}

#endif