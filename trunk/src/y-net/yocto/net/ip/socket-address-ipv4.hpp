//! \file socket-address-ipv4.hpp

#ifndef YOCTO_SOCKET_ADDRESS_IPV4_INCLUDED
#define YOCTO_SOCKET_ADDRESS_IPV4_INCLUDED

#include "socket-address-type.hpp"
#include "socket-address-format-v4.hpp"

namespace yocto {

	namespace network
	{
		typedef _socket_address<&IPv4,net32_t>  IPv4address;

	}

}

#endif
