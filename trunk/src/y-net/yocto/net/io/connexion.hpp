#ifndef YOCTO_NET_IO_CONNEXION
#define YOCTO_NET_IO_CONNEXION 1

#include "yocto/net/io/cache.hpp"
#include "yocto/net/tcp-socket.hpp"

namespace yocto
{
	
	namespace network
	{
		class connexion
		{
		public:
			explicit connexion( io_cache &cache, tcp_server &srv );
			virtual ~connexion() throw();
			
		private:
			io_cache  &qmgr;
			tcp_client sock;
			io_queue  *Q;
			
			YOCTO_DISABLE_COPY_AND_ASSIGN(connexion);
		};
	}
	
}

#endif
