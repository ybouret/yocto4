//! \file bsd-socket.hpp

#ifndef YOCTO_BSD_SOCKET_INCLUDED
#define YOCTO_BSD_SOCKET_INCLUDED 1

#include "yocto/net/network.hpp"

namespace yocto {
	
	namespace network {
		
		//! unified shutdown(...) args
		enum shutdown_mode
		{
		    shutdown_recv, //!< shutdown recv
		    shutdown_send, //!< shutdown send
		    shutdown_both  //!< shutdown recv and send
		};
		
		//! socket level options
		enum sol_socket_option
		{
			so_broadcast, 
			so_keepalive,
			so_dontroute,
			so_linger,
			so_rcvbuf,
			so_rcvlowat,
			so_reuseaddr,
			so_sndbuf,
			so_sndlowat,
			so_blocking   //!< SPECIAL
		};
		
		//! socket level option parameter
		struct sol_socket_parameter
		{
			sol_socket_option name;
			union
			{
				bool   broadcast_value;
				bool   keepalive_value;
				bool   dontroute_value;
				struct {
					bool     l_onoff;
					uint16_t l_linger; 
				}      linger_value;
				size_t rcvbuf_value;
				size_t rcvlowat_value;
				bool   reuseaddr_value;
				size_t sndbuf_value;
				size_t sndlowat_value;
				bool   blocking_value;
			} ;
		};
		
		extern const socket_t invalid_socket; //!< platform specific value
		
		struct bsd
		{
			static void       socket_close( socket_t &s ) throw();                       //!< close and set socket to invalid_socket
			static void       socket_shutdown( socket_t s, shutdown_mode sd ) throw();   //!< shutdown socket
			static socket_t   socket_bind( socket_t s, const socket_address &ip );       //!< close and throw upon failure.
			static socket_t   socket_listen( socket_t s, int max_pending);               //!< close and throw upon failure.
			static socket_t   socket_connect( socket_t s, const socket_address &ip );    //!< close and throw upon failure
			static socket_t   socket_accept( const socket_t s, socket_address &ip );     //!< new socket upon success, throw upon failure.
			
			static size_t     socket_send( socket_t s, const void *ptr, size_t len, int flags ); //!< TCP send
			static size_t     socket_recv( socket_t s, void  *ptr, size_t len, int flags);       //!< TCP recv
			
			static size_t     socket_send_to(   socket_t s, const socket_address &ip, const void *ptr, size_t len, int flags); //!< UDP send
			static size_t     socket_recv_from( socket_t s, socket_address       &ip, void *      ptr, size_t len, int flags); //!< UDP recv
			
			static void       socket_getpeername( socket_t s, socket_address &ip );            //!< for clients
			static void       set_socket_option(  socket_t s, const sol_socket_parameter & );  //!< set socket level option
			static void       get_socket_option(  socket_t s, sol_socket_parameter & );        //!< get socket level option
			static void       socket_set_blocking( socket_t s, bool b );                       //!< shortcut
			static socket_t   socket_set_default_options( socket_t s );                        //!< close and throw upon failure.
			
		};
		
		
		
	}
	
}


#endif
