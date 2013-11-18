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
		
		//! multiplexed protocol
		class protocol 
		{
		public:
			virtual ~protocol() throw();
			explicit protocol(size_t bs);
			
			virtual void on_init(connexion & ); //!< to allocate resources to connexion
			virtual void on_recv(connexion &);
			virtual void on_sent(connexion &);
			virtual void on_quit(connexion &) throw();
			virtual void on_fail(connexion &) throw(); //!< during I/O phase => deconnexion
            
			virtual void run() = 0;
			void stop() throw(); //!< set running to false
			
            
		protected:
			typedef set<socket_address,connexion> connDB;
			typedef stack<connexion>              connStack;
			socket_set   sock_db;
			connDB       conn_db;
			connStack    dropped;
			
			bool         has_recv( connexion & );
			bool         has_sent( connexion & );
			void         disconnect( connexion &) throw();
			
			bool         prepare_sock() throw(); //!< set sending
			void         process_recv();         //!< after check
			void         process_send();         //!< after process_recv
			void         kill_dropped() throw(); //!< 
			void         shutdown_all() throw(); //!< when all is done...
			
		public:
			delay        waiting; //!< default is 1 second.
			
		protected:
			io_cache     cache; 
			bool         running;
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(protocol);
		};
		
		
		//! protocol for a server
		class server_protocol : public protocol
		{
		public:
			explicit server_protocol( const socket_address &ip, int pending, size_t bs );
			virtual ~server_protocol() throw();
			
			virtual void run();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(server_protocol);
			void conn_create();
			tcp_server server;
		};
		
		
		
		
	}
}


#endif
