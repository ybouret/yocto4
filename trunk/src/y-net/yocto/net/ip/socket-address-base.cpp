#include "yocto/net/ip/socket-address-base.hpp"
#include "yocto/net/ip/socket-address-format.hpp"
#include <iostream>

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
		
		
		
		
		
		#include "socket-address-ctor.cxx"
#if defined(_MSC_VER)
#pragma warning ( disable : 4351 )
#endif
		socket_address:: socket_address( const socket_address_format &f, socket_address_type net_type, net16_t net_port ) throw() :
			fmt( f ),
			YOCTO_SOCKET_ADDRESS_CTOR()
		{
			qw_zero<items64>( addr_ );
			
			fmt.set_inaddr( host_, net_type ); // set host value
			fmt.set_family( addr_ );
			port = net_port;
			
		}
		
		socket_address:: socket_address( const socket_address &sa ) throw() :
			fmt( sa.fmt ),
			YOCTO_SOCKET_ADDRESS_CTOR()
		{
			qw_copy<items64>( addr_, sa.addr_ );
		}
			
		std::ostream & operator<<( std::ostream &os, const socket_address &sa )
		{
			sa.fmt.to_ostream( os, sa.host_ );
			return os;
		}
	
		
	}
	
}
