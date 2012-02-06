#include "yocto/net/io/protocol.hpp"

#include <iostream>

namespace yocto
{
	namespace network
	{
		
		
		protocol:: ~protocol() throw()
		{
		}
		
		protocol:: protocol( size_t bs ) :
		sock_db(),
		conn_db(),
		dropped(),
		waiting(1),
		cache( bs ),
		running(false)
		{
		}
		
		
		bool protocol:: would_send() const throw()
		{
			for( connDB::const_iterator i = conn_db.begin(); i != conn_db.end(); ++i )
			{
				const connexion &cnx = *i;
				if( cnx->sock.sending )
				{
					return true;
				}
			}
			return false;
		}
		
		
		void protocol:: on_init( connexion &cnx )
		{
			std::cerr << "[INIT] " << cnx->key() << std::endl;
		}
		
		void protocol:: on_quit( connexion &cnx )
		{
			std::cerr << "[QUIT]" << cnx->key() << std::endl;
		}
		
		
		void protocol:: on_recv( connexion &cnx )
		{
			std::cerr << "[RECV]" << cnx->key() << std::endl;
			
		}
		
		void protocol:: on_sent( connexion &cnx )
		{
			std::cerr << "[SENT]" << cnx->key() << std::endl;
			
		}
		
		
		bool protocol:: has_recv( connexion &cnx )
		{
			return cnx->ioQ.recv( cnx->cln );
		}
		
		void protocol:: disconnect( connexion &cnx ) throw()
		{
			cnx->sock.shutdown( shutdown_both );
			sock_db.remove( cnx->sock );
			(void) conn_db.remove( cnx->key() );
		}

		////////////////////////////////////////////////////////////////////////
		//
		//
		////////////////////////////////////////////////////////////////////////
		server_protocol:: server_protocol( const socket_address &ip, int pending, size_t bs ) :
		protocol( bs ),
		server( ip, pending )
		{
			//-- register server into sock_db
			sock_db.insert( server );
		}
		
		server_protocol:: ~server_protocol() throw()
		{
			
		}
		
		
		void server_protocol:: run()
		{
			static const delay no_delay(-1);
			
			running = true;
			while( running )
			{
				//==============================================================
				//
				// check if something is to be sent
				//
				//==============================================================
				const bool sending = would_send();
				delay      lasting = sending  ? no_delay : waiting;
				
				//==============================================================
				//
				// check socket set accordingly
				//
				//==============================================================
				std::cerr << "checking..." << std::endl;
				const size_t active = sock_db.check( lasting);
				if( active > 0 )
				{
					//==========================================================
					//
					// do we have server activity ?
					//
					//==========================================================
					if( sock_db.is_ready( server ) )
					{
						//------------------------------------------------------
						// new connexion !
						//------------------------------------------------------
						connexion cnx( new io_link( *this, server, cache ) );
						
						//------------------------------------------------------
						// register socket
						//------------------------------------------------------
						sock_db.insert( cnx->sock );
						
						//------------------------------------------------------
						// register connexion
						//------------------------------------------------------
						try 
						{
							if( !conn_db.insert( cnx ) )
								throw exception("server_protocol: unexpected multiple connexion!");
						}
						catch(...)
						{
							sock_db.remove( cnx->sock );
							throw;
						}
						
						//------------------------------------------------------
						// welcome it !
						//------------------------------------------------------
						try
						{
							on_init( cnx );
						}
						catch(...)
						{
							disconnect( cnx );
							throw;
						}
					}
					//==========================================================
					//
					// do we have clients activity
					//
					//==========================================================
					else
					{
						//------------------------------------------------------
						// first pass: collect incoming
						//------------------------------------------------------
						dropped.free();
						for( connDB::iterator i = conn_db.begin(); i != conn_db.end(); ++i )
						{
							//--------------------------------------------------
							// check activity
							//--------------------------------------------------
							connexion &cnx = *i;
							if( sock_db.is_ready(cnx->sock) )
							{
								//----------------------------------------------
								// something arrived ?
								//----------------------------------------------
								if( has_recv( cnx ) )
								{
									//------------------------------------------
									// yes => process
									//------------------------------------------
									on_recv( cnx );
								}
								else
								{
									//------------------------------------------
									// no => disconnecting !
									//------------------------------------------
									dropped.push( cnx );
								}
							}
						}
						
						//------------------------------------------------------
						// second pass: remove dropped
						//------------------------------------------------------
						while( dropped.size() )
						{
							connexion &cnx = dropped.peek();
							disconnect( cnx );
							dropped.pop();
						}

						//------------------------------------------------------
						// third pass: sending
						//------------------------------------------------------

						
					}
					
				}
			}
			
			
		}
		
		
	}
}
