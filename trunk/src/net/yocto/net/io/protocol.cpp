#include "yocto/net/io/protocol.hpp"

#include <iostream>

namespace yocto
{
	namespace network
	{
		
		////////////////////////////////////////////////////////////////////////
		//
		//
		// Base Protocol Implementation
		//
		//
		////////////////////////////////////////////////////////////////////////
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
		
		
		static inline void __show( const char *msg, const socket_address &ip )
		{
			std::cerr << msg << ' ' << ip << ':' << swap_be(ip.port) << std::endl;
		}
		
		void protocol:: on_init( connexion &cnx )
		{
			__show( "[INIT]", cnx->key() );
		}
		
		void protocol:: on_quit( connexion &cnx ) throw()
		{
			__show("[QUIT]",cnx->key());
		}
        
		void protocol:: on_fail( connexion &cnx ) throw()
		{
			__show("[QUIT]",cnx->key());
		}
		
		
		void protocol:: on_recv( connexion &cnx )
		{
			__show("[RECV]",cnx->key());
		}
		
		void protocol:: on_sent( connexion &cnx )
		{
			__show("[SENT]",cnx->key());
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
			on_quit( cnx );
		}
		
		void protocol:: kill_dropped() throw()
		{
			while( dropped.size() )
			{
				connexion &cnx = dropped.peek();
				disconnect( cnx );
				dropped.pop();
			}
		}
		
        
        //! count sending socket
        /**
         a closing socket would send 0 bytes.
         */
		bool protocol:: prepare_sock() throw()
		{
            bool sending = false;
			for( connDB::iterator i = conn_db.begin(); i != conn_db.end(); ++i )
			{
				connexion      &cnx  = *i;
				property<bool> &flag = cnx->cln.sending;
				if( cnx->ioQ.would_send() || cnx->closing )
				{
					flag    = true;
					sending = true;
				}
				else
				{
					flag = false;
				}
			}
            return sending;
		}
		
		void protocol:: process_recv()
		{
			//------------------------------------------------------------------
			//  collect incoming
			//------------------------------------------------------------------
			dropped.free();
			for( connDB::iterator i = conn_db.begin(); i != conn_db.end(); ++i )
			{
				//--------------------------------------------------------------
				// check activity
				//--------------------------------------------------------------
				connexion &cnx = *i;
				if( sock_db.is_ready(cnx->sock) )
				{
                    
                    try {
                        //------------------------------------------------------
                        // something arrived ?
                        //------------------------------------------------------
                        if( has_recv( cnx ) )
                        {
                            //--------------------------------------------------
                            // yes => process
                            //--------------------------------------------------
                            if( cnx->closing )
                            {
                                //-- ignore closed connexion still incoming
                                cnx->ioQ.clear_recv();
                            }
                            else
                            {
                                on_recv( cnx );
                            }
                        }
                        else
                        {
                            //--------------------------------------------------
                            // no => disconnecting !
                            //--------------------------------------------------
                            dropped.push( cnx );
                        }
                    }
                    catch(...)
                    {
                        on_fail(cnx);
                        disconnect(cnx);
                    }
                }
			}
			
			//------------------------------------------------------------------
			// second pass: remove dropped
			//------------------------------------------------------------------
			kill_dropped();
			
		}
		
		bool protocol:: has_sent( connexion &cnx )
		{
			return cnx->ioQ.sent( cnx->cln );
		}
		
		void protocol:: process_send()
		{
			dropped.free();
			for( connDB::iterator i = conn_db.begin(); i != conn_db.end(); ++i )
			{
				connexion &cnx = *i;
				if( cnx->cln.sending )
				{
					//----------------------------------------------------------
					// something to send or forced closing
					//----------------------------------------------------------
					assert( cnx->ioQ.would_send() || cnx->closing );
					if( sock_db.can_send( cnx->sock ) )
					{
                        try {
                            if( has_sent(cnx) )
                            {
                                //-- all is done
                                if(cnx->closing)
                                {
                                    //-- final sent !
                                    dropped.push(cnx);
                                }
                                else
                                {
                                    //-- what to do next ?
                                    on_sent(cnx);
                                }
                            }
                        }
                        catch(...)
                        {
                            on_fail(cnx);
                            disconnect(cnx);
                        }
					}
				}
			}
			
			kill_dropped();
		}
		
		
		void protocol:: shutdown_all() throw()
		{
			for( connDB::iterator i = conn_db.begin(); i != conn_db.end(); ++i )
			{
				(*i)->cln.shutdown( shutdown_both );
			}
			dropped.release();
			conn_db.release();
		}
		
		void protocol:: stop() throw()
		{
			running = false;
		}
		
		////////////////////////////////////////////////////////////////////////
		//
		//
		// Server Protocol Implementation
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
		
		
		void server_protocol:: conn_create()
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
                on_fail(cnx);
				disconnect( cnx );
				throw;
			}
			
		}
		
		void server_protocol:: run()
		{
			static const delay no_delay(-1);
			
			running = true;
			while( running )
			{
				//==============================================================
				// check if something is to be sent
				//==============================================================
				delay      lasting = prepare_sock()  ? no_delay : waiting;
				
				//==============================================================
				// check socket set for activity
				//==============================================================
				std::cerr << "checking..." << std::endl;
				const size_t active = sock_db.check( lasting);
				if( active > 0 )
				{
					
					if( sock_db.is_ready( server ) )
					{
						conn_create();
					}
					else
					{
						process_recv();
					}
					
				}
				
				//==============================================================
				// check sending
				//==============================================================
				process_send();
				
			}
			
			//==================================================================
			// close server and all clients
			//==================================================================
			server.shutdown(shutdown_both);
			sock_db.remove( server );
			shutdown_all();
			
		}
		
		
	}
}
