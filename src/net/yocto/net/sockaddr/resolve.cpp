#include "yocto/net/net.hpp"
#include "yocto/net/sockaddr/base.hpp"
#include "yocto/net/sockaddr/format.hpp"
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


#include <cstdlib>

namespace yocto
{
    namespace network
    {
#include "ctor.hxx"
        socket_address::socket_address(const socket_address_format &f,
                                       const string                &net_name,
                                       net16_t                      net_port  ) :
        fmt( f ),
        YOCTO_SOCKADDR_CTOR()
        {
            memset(addr_,0,sizeof(addr_));
            fmt.set_family( addr_ );
            port = net_port;

            if( 0 == net_port )
            {
                char *       col     = (char *)net_name.c_str();
                bool         has_col = false;
                for(size_t i=net_name.size(); i>0; --i, ++col )
                {
                    if( ':' == *col )
                    {
                        has_col = true;
                        break;
                    }
                }

                if( has_col )
                {
                    net_port = uint16_t( atoi( col+1 ) );     //TODO: better
                    port     = swap_be( net_port );
                    *col     = 0; // bad, bad hack
                    try {
                        net::instance().resolve( *this, net_name );
                        *col = ':'; // revert hack
                    }
                    catch(...)
                    {
                        *col = ':';
                        throw; // revert hack
                    }
                    
                }
                else
                {
                    net::instance().resolve( *this, net_name );
                }
            }
            else
            {
                net::instance().resolve( *this, net_name );
            }
            
        }

    }
}
