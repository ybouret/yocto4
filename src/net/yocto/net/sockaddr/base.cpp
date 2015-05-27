#include "yocto/net/sockaddr/base.hpp"
#include "yocto/net/sockaddr/format.hpp"

#include <iostream>
#include <cstring>

namespace yocto
{
    namespace network
    {

        socket_address:: ~socket_address() throw()
        {
        }


        const void * socket_address:: get_address() const throw()
        {
            return &addr_[0];
        }

        size_t socket_address:: length() const throw()
        {
            return fmt.addrlen;
        }

        bool operator==( const socket_address &lhs, const socket_address &rhs ) throw()
        {
            return lhs.port == rhs.port && 0 == memcmp( lhs.addr_, rhs.addr_, socket_address::storage  );
        }

        bool operator!=( const socket_address &lhs, const socket_address &rhs ) throw()
        {
            return lhs.port != rhs.port || 0 != memcmp( lhs.addr_, rhs.addr_, socket_address::storage );
        }


#include "ctor.hxx"


        socket_address:: socket_address( const socket_address_format &f, socket_address_type net_type, net16_t net_port ) throw() :
        fmt( f ),
        YOCTO_SOCKADDR_CTOR()
        {
            memset(addr_,0,sizeof(addr_));

            fmt.set_inaddr( host_, net_type ); // set host value
            fmt.set_family( addr_ );
            port = net_port;

        }

        socket_address:: socket_address( const socket_address &sa ) throw() :
        fmt( sa.fmt ),
        YOCTO_SOCKADDR_CTOR()
        {
            memcpy(addr_, sa.addr_, sizeof(sa.addr_) );
        }
        
        std::ostream & operator<<( std::ostream &os, const socket_address &sa )
        {
            sa.fmt.to_ostream( os, sa.host_ );
            return os;
        }
        
        
    }
    
}
