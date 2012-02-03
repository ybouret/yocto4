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
		
		
		
	}
}
