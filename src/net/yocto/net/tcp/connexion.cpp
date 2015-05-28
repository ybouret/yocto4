#include "yocto/net/tcp/connexion.hpp"

namespace yocto
{
    namespace network
    {

        connexion:: ~connexion() throw()
        {
        }


#define  Y_CNX_CTOR()\
closed(false)

        connexion:: connexion(const socket_address &ipaddr,
                              io_cache             &shared ) :
        client(ipaddr),
        Y_CNX_CTOR(),
        recvQ(shared),
        sendQ(shared)
        {
            
        }

        
    }
    
}



