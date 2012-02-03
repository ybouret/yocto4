#include "yocto/net/io/connexion.hpp"

namespace yocto
{
	namespace network
	{
		
		connexion:: ~connexion() throw()
		{
			qmgr.collect(Q);
			Q = NULL;
		}
		
		
		connexion:: connexion( io_cache &cache, tcp_server &srv ) :
		qmgr( cache ),
		sock( srv   ),
		Q( qmgr.provide() )
		{
		}
		
		
	}
}
