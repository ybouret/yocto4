//! \file udp-socket.hpp

#ifndef YOCTO_UDP_SOCKET_INCLUDED
#define YOCTO_UDP_SOCKET_INCLUDED 1

#include "socket.hpp"
#include "io-socket.hpp"

namespace yocto 
{

	namespace network
	{

		//! low-level UDP socket
		class udp_socket :
			public socket, public io_socket
		{
		public:
			virtual ~udp_socket() throw();

			socket_address       &peer() throw();       //!< change peer address
			const socket_address &peer() const throw(); //!< access peer address


			virtual size_t send( const void *ptr, size_t len);    //!< UDP send datagram
			virtual size_t recv( void       *ptr, size_t len);    //!< UDP recv datagram


		protected:
			explicit udp_socket( const socket_address &ip, type2type<udp_client> ) throw(); //!< initialize an UDP client
			explicit udp_socket( const socket_address &ip, type2type<udp_server> ) throw(); //!< initialize an UDP server

		private:
			virtual socket_address &peer_address_ref() const throw() = 0;
			YOCTO_DISABLE_COPY_AND_ASSIGN(udp_socket);
		};

		//! UDP client
		class udp_client: public udp_socket {
		public:
			explicit udp_client();
			//! constructor
			/**
			 *  \param peer_ip address to send/recv datagram.
			 */
			explicit udp_client( const socket_address &peer_ip );
			virtual ~udp_client() throw(); //!< default destructor


		private:
			virtual socket_address &peer_address_ref() const throw(); //!< uses ipaddr_ as peer
			YOCTO_DISABLE_COPY_AND_ASSIGN(udp_client);
		};

		//! UDP server
		class udp_server: public udp_socket {
		public:
			//! constructor
			/**
			 *  \param ip address to bind.
			 */
			explicit udp_server( const socket_address &ip );
			virtual ~udp_server() throw();

		private:
			virtual socket_address &peer_address_ref() const throw(); //!< uses peer_ as peer
			socket_address peer_;
			YOCTO_DISABLE_COPY_AND_ASSIGN(udp_server);
		};


	}

}

#endif
