#ifndef YOCTO_SOCKET_ADDRESS_IPV6_INCLUDED
#define YOCTO_SOCKET_ADDRESS_IPV6_INCLUDED

#include "socket-address-type.hpp"
#include "socket-address-format-v6.hpp"

namespace yocto {

	namespace network
	{
		typedef _socket_address<&IPv6,net128_t>  IPv6address;

	}

}

#endif
