#include "yocto/net/io/connexion.hpp"

namespace yocto
{
	namespace network
	{
		
		io_link:: ~io_link() throw()
		{
			mgr.collect( ioQ );
			ioQ = NULL;
		}
		
		io_link:: io_link( io_cache &cache, tcp_server &srv, bool blocking_status ) :
		mgr( cache ),
		cln( srv   ),
		ioQ( mgr.provide() ),
		
		sock(    cln ),
		input(  *ioQ ),
		output( *ioQ )
		{
			cln.blocking( blocking_status );
		}
	
		const socket_address & io_link:: key() const throw()
		{
			return cln.self();
		}
		
	}
}
