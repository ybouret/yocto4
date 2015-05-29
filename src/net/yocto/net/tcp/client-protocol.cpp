#include "yocto/net/tcp/client-protocol.hpp"

namespace yocto
{
    namespace network
    {

        client_protocol:: ~client_protocol() throw()
        {
        }


        client_protocol:: client_protocol(const size_t block_size) :
        tcp_protocol(block_size),
        running(false)
        {
        }
        

    }

}

