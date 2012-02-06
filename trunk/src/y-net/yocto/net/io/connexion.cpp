#include "yocto/net/io/connexion.hpp"

namespace yocto
{
	namespace network
	{
		
		io_link:: ~io_link() throw()
		{
			
		}
		
		io_link:: io_link( protocol &p, tcp_server &srv, io_cache &cache ) :
		cln( srv   ),
		ioQ( cache ),
		closing( false ),
		sock(   cln ),
		input(  ioQ ),
		output( ioQ ),
		proto(p)
		{
			cln.blocking( false );
		}
	
		const socket_address & io_link:: key() const throw()
		{
			return cln.self();
		}
		
	}
}
