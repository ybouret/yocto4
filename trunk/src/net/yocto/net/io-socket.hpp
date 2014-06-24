#ifndef YOCTO_IO_SOCKET_INCLUDED
#define YOCTO_IO_SOCKET_INCLUDED 1

#include "yocto/ios/ichannel.hpp"
#include "yocto/ios/ochannel.hpp"

namespace yocto
{
	namespace network
	{
		
		//! virtual socket to send/recv
		class io_socket : 
		public ios::ichannel,
		public ios::ochannel
		{
		public:
			virtual ~io_socket() throw();
			
			virtual size_t send( const void *, size_t ) = 0; //!< TCP send or UDP sendto
			virtual size_t recv( void       *, size_t ) = 0; //!< TCP recv or UDP recvfrom
			
			
			
			virtual void   put( const void *ptr, size_t len, size_t &done ); //!< ios::ochannel implementation
			virtual size_t get( void       *ptr, size_t len); //!< ios::ichannel implementation
			
			
		protected:
			explicit io_socket() throw();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(io_socket);
			
		};
	}
	
}


#endif
