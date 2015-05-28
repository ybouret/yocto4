#ifndef YOCTO_NET_TCP_CONNEXION_INCLUDED
#define YOCTO_NET_TCP_CONNEXION_INCLUDED 1

#include "yocto/net/socket/tcp.hpp"
#include "yocto/net/io/queue.hpp"

namespace yocto
{
    namespace network
    {

        //! an asynchronous TCP connexion
        class connexion : public tcp_client
        {
        public:
            explicit connexion( const socket_address &ipaddr, io_cache &shared );
            explicit connexion( tcp_server           &src,    io_cache &shared );
            virtual ~connexion() throw();

            void       close() throw();
            
            const bool closed;
            io_queue   recvQ;
            io_queue   sendQ;


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(connexion);
        };
        
    }

}


#endif
