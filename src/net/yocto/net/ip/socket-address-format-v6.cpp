
#include "yocto/net/ip/socket-address-format-v6.hpp"
#include "yocto/net/ip/socket-address-sys.hpp"
#include "yocto/core/offset.hpp"

#include <iostream>
#include <cstring>

#if defined(YOCTO_BSD)
#include <sys/socket.h>
#endif

namespace yocto {
	
	namespace network 
	{
		
		static void ipv6_set_family( void *addr ) throw()
		{
			assert( addr );
			static_cast<sockaddr_in6 *>(addr)->sin6_family = AF_INET6;
		}
		
		
		static const uint8_t ipv6_none[16] =
		{
			0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
            0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
			
		};
		
		static const uint8_t ipv6_any[16] =
		{
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
		};
		
		static const uint8_t ipv6_loopback[16] =
		{
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
		};
		
		
		
		static
		void ipv6_set_inaddr( void *host, socket_address_type type ) throw()
		{
			assert( host );
			switch( type )
			{
				case socket_address_none:
					memcpy( host, ipv6_none, 16 );
					break;
					
				case socket_address_any:
					memcpy( host, ipv6_any, 16 );
					break;
					
				case socket_address_loopback:
					memcpy( host, ipv6_loopback, 16 );
					break;
			}
		}
		
		
		static
		void ipv6_to_ostream( std::ostream &os, const void *host )
		{
			const uint8_t *h = (const uint8_t *)host;
			if( h[0] )
				os << int(h[0]);
			os << ':';
			
			for( size_t i=1; i<15 ; ++i )
			{
				if(h[i])
					os << int(h[i]) << ':';
			}
			
			os << ':';
            if( h[15])
				os<< int(h[15]);
		}
		
		const socket_address_format IPv6 =
		{
			sizeof(sockaddr_in6),
			sizeof(net128_t),
            AF_INET6,
			PF_INET6,
			YOCTO_OFFSET_OF(sockaddr_in6,sin6_addr),
			YOCTO_OFFSET_OF(sockaddr_in6,sin6_port),
			ipv6_set_family,
			ipv6_set_inaddr,
			ipv6_to_ostream,
			"IPv6"
		};
		
	}
	
}
