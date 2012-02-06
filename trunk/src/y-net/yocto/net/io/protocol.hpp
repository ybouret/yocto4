#ifndef YOCTO_NETWORK_IO_PROTOCOL_INCLUDED
#define YOCTO_NETWORK_IO_PROTOCOL_INCLUDED 1

#include "yocto/net/io/connexion.hpp"
#include "yocto/net/socket-set.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/stock/stack.hpp"

namespace yocto
{
	namespace network
	{
		
		class protocol 
		{
		public:
			virtual ~protocol() throw();
			explicit protocol(size_t bs);

			virtual void on_init(connexion & );
			virtual void on_recv(connexion &);
			virtual void on_sent(connexion &);
			virtual void on_quit(connexion &);
			
			bool would_send() const throw();
			
		protected:
			typedef set<socket_address,connexion> connDB;
			typedef stack<connexion>              connStack;
			socket_set   sock_db;
			connDB       conn_db;
			connStack    dropped;
			bool         has_recv( connexion & );
			bool         has_sent( connexion & );
			
			
		public:
			delay        waiting;
			
		protected:
			io_cache     cache; 
			bool         running;
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(protocol);
		};
		
	
		class server_protocol : public protocol
		{
		public:
			explicit server_protocol( const socket_address &ip, int pending, size_t bs );
			virtual ~server_protocol() throw();
			
			void run();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(server_protocol);
			tcp_server server;
		};
		
		
	}
}


#endif
