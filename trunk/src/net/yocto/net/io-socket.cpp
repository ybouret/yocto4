#include "yocto/net/io-socket.hpp"

namespace yocto
{
	namespace network
	{
		
		io_socket ::  io_socket() throw() {}
		io_socket :: ~io_socket() throw() {}
		
		void io_socket:: put( const void *ptr, size_t len, size_t &done ) 
		{
			done = 0;
			done = this->send( ptr, len );
		}
		
		void io_socket:: get( void *ptr, size_t len, size_t &done )
		{
			done = 0;
			done = this->recv( ptr, len );
		}
		
		
	}
	
}
