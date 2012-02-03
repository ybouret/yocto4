#ifndef YOCTO_NET_IO_CONNEXION
#define YOCTO_NET_IO_CONNEXION 1

#include "yocto/net/io/cache.hpp"
#include "yocto/net/tcp-socket.hpp"
#include "yocto/intrusive-ptr.hpp"

namespace yocto
{
	
	namespace network
	{
		
		class io_link : public object, public counted
		{
		public:
			explicit io_link( io_cache &cache, tcp_server &srv, bool blocking = true );
			virtual ~io_link() throw();
			
		private:
			io_cache   &mgr;
			tcp_client  cln;
			io_queue   *ioQ;
			YOCTO_DISABLE_COPY_AND_ASSIGN(io_link);
			
		public:
			socket       &sock;
			ios::istream &input;
			ios::ostream &output;
			
			const socket_address & key() const throw();
		};
		
		typedef intrusive_ptr<socket_address,io_link> connexion;
		
	}
	
}

#endif
