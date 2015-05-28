#ifndef YOCTO_NET_TCP_PROTOCOL_INCLUDED
#define YOCTO_NET_TCP_PROTOCOL_INCLUDED 1

#include "yocto/net/tcp/connexion.hpp"
#include "yocto/net/socket/set.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/ptr/intr.hpp"

namespace yocto
{
    namespace network
    {

        class protocol
        {
        public:

            explicit protocol( const socket_address &ipaddr, const int max_pending=2);
            virtual ~protocol() throw();

            

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(protocol);
            typedef intr_ptr<sock_key_t,connexion> conn_ptr;
            typedef set<sock_key_t,conn_ptr>       conn_set;

            tcp_server server;
            socket_set sockset;
            conn_set   conn_db;
            
        };


    }

}

#endif
