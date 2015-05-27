#ifndef YOCTO_SOCKADDR_IPV4_INCLUDED
#define YOCTO_SOCKADDR_IPV4_INCLUDED

#include "yocto/net/sockaddr/type.hpp"
#include "yocto/net/sockaddr/format-ipv4.hpp"

namespace yocto {

    namespace network
    {
        typedef _socket_address<&IPv4FORMAT,net32_t>  IPv4;
        
    }
    
}

#endif
