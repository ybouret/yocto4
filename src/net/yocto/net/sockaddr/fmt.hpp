#ifndef YOCTO_NET_SOCKADDR_FMT_INCLUDED
#define YOCTO_NET_SOCKADDR_FMT_INCLUDED 1

#include "yocto/net/sockaddr/base.hpp"

namespace yocto {

    namespace network
    {

        //! platform specific address format
        struct socket_address_format
        {
            size_t addrlen;  //!< sizeof( sockaddr_in[6] )
            size_t hostlen;  //!< sizeof(net32_t|net128_t)
            int    af;       //!< AF_INET[6]
            int    pf;       //!< PF_INET[6]

            size_t host_offset; //!< *host location
            size_t port_offset; //!< *port location

            void (*set_family)( void *addr ) throw();
            void (*set_inaddr)( void *host, socket_address_type ) throw();
            void (*to_ostream)( std::ostream &, const void *host );
            
            const char *version;
            
        };
        
        
    }
    
}


#endif

