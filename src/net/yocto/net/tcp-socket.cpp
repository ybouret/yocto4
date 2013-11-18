#include "yocto/net/tcp-socket.hpp"

namespace yocto {
	
	namespace network {
		//==========================================================================
		//
		// tcp_server
		//
		//==========================================================================
		tcp_server:: tcp_server( const socket_address &ip, const int max_pending) :
		socket( ip, max_pending, type2type<tcp_server>() )
		{
			
		}
		
		tcp_server:: ~tcp_server() throw() {
			this->shutdown( shutdown_both );
		}
		
		socket_t tcp_server:: get_socket() const throw() 
		{
			return socket_;
		}
		
		
		
		//==========================================================================
		//
		// tcp_client
		//
		//==========================================================================
		tcp_client:: tcp_client( const socket_address &ip ) :
		socket( ip, type2type<tcp_client>() )
		{
			
		}
		
		tcp_client:: ~tcp_client() throw() {
			
		}
		
		tcp_client:: tcp_client( tcp_server &srv ) :
		socket( srv.self().fmt, srv.get_socket(), type2type<tcp_client>() )
		{
			
		}
		
		size_t tcp_client:: send( const void *ptr, size_t len ) 
		{
			assert(!(ptr==NULL&&len>0));
			return bsd::socket_send( socket_, ptr, len, ioflag_ );
		}
		
		
		
		size_t tcp_client:: recv( void *ptr, size_t len ) 
		{
			assert(!(ptr==NULL&&len>0));
			return bsd::socket_recv( socket_, ptr, len, ioflag_ );
		}
		
		
		
	}
	
}
