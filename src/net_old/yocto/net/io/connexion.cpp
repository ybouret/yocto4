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
		
		void io_link:: close() throw()
		{
			closing = true;
		}
		
		size_t io_link:: input_cache() const throw()
		{
			return ioQ.recv_length;
		}
        
        
        ////////////////////////////////////////////////////////////////////////
        tcp_link:: ~tcp_link() throw() {}
        tcp_link:: tcp_link( const connexion &cnx ) throw() : 
        key_( cnx->key() )
        {}
        
        const socket_address & tcp_link:: key() const throw() { return key_; }
        
        
	}
}
