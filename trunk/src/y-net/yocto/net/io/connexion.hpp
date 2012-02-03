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
			explicit connexion();
			virtual ~connexion() throw();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(connexion);
		};
	}
	
}

#endif
