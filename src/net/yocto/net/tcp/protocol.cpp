#include "yocto/net/tcp/protocol.hpp"

namespace yocto
{
    namespace network
    {

        protocol:: ~protocol() throw()
        {
        }

        protocol:: protocol( const socket_address &ipaddr, const int max_pending) :
        server(ipaddr,max_pending),
        sockset(),
        conn_db()
        {

            sockset.insert(server);
            
        }

        

    }

}
