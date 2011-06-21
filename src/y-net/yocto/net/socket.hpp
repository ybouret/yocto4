//! \file socket.hpp

#ifndef YOCTO_SOCKET_INCLUDED
#define YOCTO_SOCKET_INCLUDED 1

#include "yocto/net/sys/bsd-socket.hpp"
#include "yocto/net/ip/socket-address-format.hpp"
#include "yocto/design/property.hpp"

namespace  yocto {

	namespace network {

		class tcp_client;
		class tcp_server;
		class udp_client;
		class udp_server;


		class sndppty : public property<bool>
		{
		public:
			explicit sndppty( bool &sending, bool rw ) throw();
			virtual ~sndppty() throw();
			virtual const char *name() const throw();
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(sndppty);
		};
		
		//! bsd sockets wrapper
		class socket : public virtual object
		{
		public:
			virtual ~socket() throw();

			void shutdown(const shutdown_mode sd ) throw(); //!< manual shutdown.
			void blocking( bool do_block ) throw();         //!< manual blocking status.

			const socket_address &self() const throw(); //!< associated address

			ptrdiff_t fd_value() const throw(); //!< bsd::socket_

			
		protected:

			//------------------------------------------------------------------
			// tcp server
			//------------------------------------------------------------------
			//! create tcp, set options, bind to ip address, and listen with max_pending connections.
			explicit socket( const socket_address &ip, const int max_pending, type2type<tcp_server> );

			//------------------------------------------------------------------
			// tcp client, connected
			//------------------------------------------------------------------
			//! create tcp, set options, connect to ip address.
			explicit socket( const socket_address &ip, type2type<tcp_client> );

			//------------------------------------------------------------------
			// tcp client, accepted
			//------------------------------------------------------------------
			//! format socket_address_ with srv_addr, accept with srv_sock, set options.
			explicit socket( const socket_address_format &f, socket_t srv_sock, type2type<tcp_client> );

			//------------------------------------------------------------------
			// udp client
			//------------------------------------------------------------------
			//! create udp, set options
			explicit socket( const socket_address &ip, type2type<udp_client> );

			//------------------------------------------------------------------
			// udp client
			//------------------------------------------------------------------
			//! create udp, set options, bind to ip address
			explicit socket( const socket_address &ip, type2type<udp_server> );


			socket_address ipaddr_;  //!< generic ip address, depends on initialization.
			socket_t       socket_;  //!< O/S interface.
			int            ioflag_;  //!< socket api I/O flags, default=0
			bool           sending_; //!< internal flag
			
		public:
			sndppty        sending; //!< user controled: true => check if ready to write in socket_set

			size_t         sndbuf(void) const throw(); //!< hardware sendbuf
			size_t         rcvbuf(void) const throw(); //!< hardware recvbuf

		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(socket);
			friend class socket_set;

		};

	}

}

#endif
