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
		
		size_t io_socket:: get( void *ptr, size_t len )
		{
			return this->recv( ptr, len );
		}
		
		
	}
	
}
