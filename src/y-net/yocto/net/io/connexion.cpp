#include "yocto/net/io/connexion.hpp"

namespace yocto
{
	namespace network
	{
		
		io_link:: ~io_link() throw()
		{
			
		}
		
		io_link:: io_link( io_cache &cache, tcp_server &srv ) :
		cln( srv   ),
		ioQ( cache ),
		
		sock(   cln ),
		input(  ioQ ),
		output( ioQ )
		{
			cln.blocking( false );
		}
	
		const socket_address & io_link:: key() const throw()
		{
			return cln.self();
		}
		
	}
}
