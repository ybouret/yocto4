#include "yocto/net/io/protocol.hpp"


namespace yocto
{
	namespace network
	{
		
		
		protocol:: ~protocol() throw()
		{
		}
		
		protocol:: protocol( size_t bs ) :
		sock_db(),
		conn_db(),
		waiting(1),
		cache( bs )
		{
		}
		
		
		server_protocol:: server_protocol( const socket_address &ip, int pending, size_t bs ) :
		protocol( bs ),
		server( ip, pending )
		{
		}
		
		server_protocol:: ~server_protocol() throw()
		{
			
		}
		
		
		
	}
}
