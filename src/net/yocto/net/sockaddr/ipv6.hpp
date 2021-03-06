#ifndef YOCTO_SOCKADDR_IPV6_INCLUDED
#define YOCTO_SOCKADDR_IPV6_INCLUDED

#include "yocto/net/sockaddr/type.hpp"
#include "yocto/net/sockaddr/format-ipv6.hpp"

namespace yocto {

    namespace network
    {
        typedef _socket_address<&IPv6FORMAT,net128_t>  IPv6;

    }

}

#endif
