#ifndef YOCTO_NET_TCP_CONNEXION_INCLUDED
#define YOCTO_NET_TCP_CONNEXION_INCLUDED 1

#include "yocto/net/socket/tcp.hpp"
#include "yocto/net/io/queue.hpp"

namespace yocto
{
    namespace network
    {

        //! an asynchronous TCP connexion
        class connexion : public object
        {
        public:
            explicit connexion( const socket_address &ipaddr, io_cache &shared );
            virtual ~connexion() throw();

            


        private:
            tcp_client client;
            bool       closed;
            io_queue   recvQ;
            io_queue   sendQ;

            YOCTO_DISABLE_COPY_AND_ASSIGN(connexion);
        };
        
    }

}


#endif
