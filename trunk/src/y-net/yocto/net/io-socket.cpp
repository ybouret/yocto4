#include "yocto/net/io-socket.hpp"

namespace yocto
{
	namespace network
	{
		
		io_socket ::  io_socket() throw() {}
		io_socket :: ~io_socket() throw() {}
		
		void io_socket:: do_put( const void *ptr, size_t len, size_t &done ) 
		{
			assert( 0 == done );
			done = this->send( ptr, len );
		}
		
		void io_socket:: do_get( void *ptr, size_t len, size_t &done )
		{
			assert( 0 == done );
			done = this->recv( ptr, len );
		}
		
		
	}
	
}
