#include "yocto/net/udp-socket.hpp"

namespace yocto {

	namespace network {
		//==========================================================================
		//
		// udp_socket
		//
		//==========================================================================
		udp_socket:: udp_socket( const socket_address &ip, type2type<udp_server> ) throw() :
				socket( ip, type2type<udp_server>() )
		{
		}

		udp_socket:: udp_socket( const socket_address &ip, type2type<udp_client> ) throw() :
				socket( ip, type2type<udp_client>() )
		{
		}



		udp_socket:: ~udp_socket() throw() {
		}


		const socket_address &udp_socket:: peer(void) const throw() {
			return peer_address_ref();
		}

		socket_address &udp_socket:: peer(void) throw() {
			return peer_address_ref();
		}

		size_t udp_socket:: send( const void *ptr, const size_t len) {
			assert(!(ptr==NULL&&len>0));
			return bsd::socket_send_to( socket_, peer_address_ref(), ptr, len, ioflag_ );
		}

		size_t udp_socket:: recv( void *ptr, const size_t len) {
			assert(!(ptr==NULL&&len>0));
			return bsd::socket_recv_from( socket_, peer_address_ref(), ptr, len, ioflag_ );
		}


		//======================================================================
		//
		// udp_ client
		//
		//======================================================================

		udp_client:: udp_client( const socket_address &ip ) :
				udp_socket( ip, type2type<udp_client>() )
		{

		}

		udp_client:: ~udp_client() throw() {

		}


		socket_address &udp_client:: peer_address_ref() const throw() {
			return *(socket_address *)&ipaddr_;
		}

		//======================================================================
		//
		// udp_ server
		//
		//======================================================================
		udp_server:: udp_server( const socket_address &ip ) :
				udp_socket( ip, type2type<udp_server>() ),
				peer_( ip.fmt, socket_address_none, 0 )
		{

		}

		udp_server:: ~udp_server() throw() {

		}

		socket_address &udp_server:: peer_address_ref() const throw() {
			return *(socket_address *)&peer_;
		}

	}

}
