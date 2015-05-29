#include "yocto/net/tcp/server-protocol.hpp"


namespace yocto
{
    namespace network
    {
        server_protocol:: ~server_protocol() throw()
        {
        }

        server_protocol:: server_protocol(const socket_address &ipaddr, const int max_pending, const size_t block_size ) :
        tcp_protocol(block_size),
        server(ipaddr,max_pending)
        {
            sockset.insert(server);
        }

        void server_protocol:: suspend() throw()
        {
            running = false;
        }
    }
}


namespace yocto
{
    namespace network
    {
        void server_protocol:: execute()
        {
            running = true;
            while(true)
            {
                //______________________________________________________________
                //
                // do we have someting to send
                //______________________________________________________________
                double          delay_value       = every;
                if( initialize() )
                {
                    delay_value = 0;
                }
                else
                {
                    // nothing to send...
                    if(!running)
                    {
                        return;
                    }
                }

                //______________________________________________________________
                //
                // network scanning
                //______________________________________________________________
                delay            d = delay_value;
                const size_t     n = sockset.check(d);
                std::cerr << server.self() << ": #activity=" << n << std::endl;
                if(n>0)
                {
                    check_recv();
                    check_conn();
                    check_send();
                }

                //______________________________________________________________
                //
                // other things to do
                //______________________________________________________________
                onIdle();
            }

        }


    }
}

#include "yocto/exception.hpp"

namespace yocto
{
    namespace network
    {
        void server_protocol:: check_conn()
        {
            //__________________________________________________________________
            //
            // Check server status and create new connection
            //__________________________________________________________________
            if( sockset.is_ready(server) )
            {
                conn_ptr         cnx( new connexion(server,cache) );
                const sock_key_t k  = cnx->key();
                if(!conn_db.insert(cnx))
                {
                    throw exception("invalid new connexion key!");
                }
                try
                {
                    sockset.insert(*cnx);
                }
                catch(...)
                {
                    (void)conn_db.remove(k);
                    throw;
                }
                
                if(!running)
                {
                    cnx->close();
                    return;
                }

                try
                {
                    //----------------------------------------------------------
                    //
                    // PROCESS CONNECT
                    //
                    //----------------------------------------------------------
                    std::cerr << "connexion from " << cnx->self() << ":" << int(swap_be(cnx->self().port)) << std::endl;
                    onInit(*cnx);
                }
                catch(...)
                {
                    sockset.remove(*cnx);
                    (void)conn_db.remove(k);
                    throw;
                }
            }
            
        }
        
    }
    
}


