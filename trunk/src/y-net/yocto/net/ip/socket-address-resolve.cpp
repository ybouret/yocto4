#include "yocto/net/ip/socket-address-base.hpp"
#include "yocto/net/ip/socket-address-format.hpp"
#include "yocto/net/network.hpp"
#include <cstdlib>
#include <cstring>

namespace yocto {

	namespace network
	{

#include "socket-address-ctor.cxx"

#if defined(_MSC_VER)
#pragma warning ( disable : 4351 )
#endif
		socket_address::socket_address( const socket_address_format &f, const string &net_name, net16_t net_port  ) :
	fmt( f ),
		YOCTO_SOCKET_ADDRESS_CTOR()
	{
		memset(addr_,0,sizeof(addr_));
		//qw_zero<items64>( addr_ );
		fmt.set_family( addr_ );
		port = net_port;

		if( 0 == net_port )
		{
			char *       col = (char *)&net_name[0];
			bool         has_col = false;
			for(size_t i= net_name.size(); i>0; --i, ++col )
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
				net::instance().resolve( *this, net_name );
		}
		else
			net::instance().resolve( *this, net_name );
	}



	}

}
