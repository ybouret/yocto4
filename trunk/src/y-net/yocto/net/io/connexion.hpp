#ifndef YOCTO_NET_IO_CONNEXION
#define YOCTO_NET_IO_CONNEXION 1

#include "yocto/net/io/queue.hpp"
#include "yocto/net/tcp-socket.hpp"
#include "yocto/intrusive-ptr.hpp"

namespace yocto
{
	
	namespace network
	{
		
		class protocol;
		class io_link : public object, public counted
		{
		public:
			explicit io_link( protocol &p, tcp_server &srv, io_cache &cache);
			virtual ~io_link() throw();
			
		private:
			tcp_client  cln;
			io_queue    ioQ;
			bool        closing;
			
			YOCTO_DISABLE_COPY_AND_ASSIGN(io_link);
			friend class protocol;

		public:
			socket       &sock;
			ios::istream &input;
			ios::ostream &output;
			protocol     &proto;
			void          close() throw();             //!< never come back
			size_t        input_cache() const throw(); //!< ready for input
			
			const socket_address & key() const throw();
		};
		
		typedef intrusive_ptr<socket_address,io_link> connexion;
		
	}
	
}

#endif
