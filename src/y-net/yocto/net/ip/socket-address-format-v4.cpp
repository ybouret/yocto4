#include "yocto/net/ip/socket-address-format-v4.hpp"
#include "yocto/net/ip/socket-address-sys.hpp"
#include <iostream>
#include "yocto/core/offset.hpp"

#if defined(YOCTO_BSD)
#include <sys/socket.h>
#endif

namespace yocto {
	
	namespace network {
		
		static
		void ipv4_set_family( void *addr ) throw()
		{
			assert( addr   );
			static_cast<sockaddr_in *>( addr )->sin_family = AF_INET;
		}
		
		static
		void ipv4_set_inaddr( void *host, socket_address_type type ) throw()
		{
			assert( host );
			switch( type )
			{
				case socket_address_none:
					*(net32_t *)host = INADDR_NONE;
					break;
					
				case socket_address_any:
					*(net32_t *)host = INADDR_ANY;
					break;
					
				case socket_address_loopback:
					*(net32_t *)host = INADDR_LOOPBACK;
					break;
			}
		}
		
		static
		void ipv4_to_ostream( std::ostream &os, const void *host ) throw()
		{
			const uint8_t *h = (const uint8_t *)host;
            os << int(h[0]) << '.' << int(h[1]) << '.' << int(h[2]) << '.' << int(h[3]);
			
		}
		
		const socket_address_format IPv4 =
		{
			sizeof(sockaddr_in),
			sizeof(net32_t),
			AF_INET,
			PF_INET,
			YOCTO_OFFSET_OF(sockaddr_in,sin_addr.s_addr),
			YOCTO_OFFSET_OF(sockaddr_in,sin_port),
			ipv4_set_family,
			ipv4_set_inaddr,
			ipv4_to_ostream,
			"IPv4"
		};
		
	}
	
}
