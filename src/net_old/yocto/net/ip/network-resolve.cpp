#include "yocto/net/network.hpp"
#include "yocto/net/ip/socket-address-base.hpp"
#include "yocto/net/ip/socket-address-format.hpp"
#include "yocto/exceptions.hpp"

#if defined(YOCTO_BSD)
#include <netdb.h>
#include "yocto/memory/global.hpp"
#endif

#if defined(YOCTO_WIN)
#include "yocto/memory/global.hpp"

#	if defined(__DMC__)
extern "C" {
    
	struct addrinfo {
		int ai_flags;
		int ai_family;
		int ai_socktype;
		int ai_protocol;
		size_t ai_addrlen;
		char *ai_canonname;
		struct sockaddr *ai_addr;
		struct addrinfo *ai_next;
	};
    
	int WSAAPI getaddrinfo(
                           const char *nodename,
                           const char *servname,
                           const struct addrinfo *fmt,
                           struct addrinfo **res
                           );
    
	void WSAAPI freeaddrinfo( struct addrinfo *ai );
    
    
}
#	endif // __DMC__

#endif

#include <cstring>


namespace yocto {
    
	namespace network {
        
        
		void net:: resolve( socket_address &ip, const string &s ) const
		{
			YOCTO_LOCK(access);
			const char        *name = s.c_str();
            
			addrinfo fmt;
			memset( &fmt, 0, sizeof(fmt) );
			fmt.ai_family = ip.fmt.af;
            
			YOCTO_GIANT_LOCK();
            
			addrinfo *ai0 = NULL;
			int       err = ::getaddrinfo(name, NULL, &fmt, &ai0);
			if( err )
			{
#				if defined(YOCTO_WIN)
				throw win32::exception( err, "::getaddrinfo(%s,%s)" , name, ip.fmt.version );
#				endif
                
#				if defined(YOCTO_BSD)
				throw imported::exception( gai_strerror(err), "::getaddrinfo(%s,%s)" , name, ip.fmt.version );
#				endif
			}
            
			assert( ai0             != NULL        );
			assert( ai0->ai_addr    != NULL        );
			assert( ai0->ai_addrlen == ip.length() );
            
			const net16_t p = ip.port; //-- save port
			memcpy( ip.rw(), ai0->ai_addr, ip.length() );
			ip.port = p;               //-- restore port
            
			::freeaddrinfo(ai0);
            
		}
        
        
        
	} // network
    
} // yocto			
