#include "yocto/net/tcp/connexion.hpp"

namespace yocto
{
    namespace network
    {

        connexion:: ~connexion() throw()
        {
            
        }

#define  Y_CNX_CTOR() \
closed(false),        \
recvQ(shared),        \
sendQ(shared)

        connexion:: connexion(const socket_address &ipaddr,
                              io_cache             &shared ) :
        tcp_client(ipaddr),
        Y_CNX_CTOR()
        {

        }

        connexion:: connexion(tcp_server           &srv,
                              io_cache             &shared ) :
        tcp_client(srv),
        Y_CNX_CTOR()
        {

        }


        void connexion:: close() throw()
        {
            (bool &)closed = true;
            shutdown(shutdown_recv);
        }


        size_t connexion:: do_recv()
        {
            return recvQ.recv(*this);
        }
        
        size_t connexion:: do_send()
        {
            return sendQ.send(*this);
        }
        
    }
    
}



