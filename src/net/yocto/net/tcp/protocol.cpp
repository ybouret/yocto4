#include "yocto/net/tcp/protocol.hpp"
#include "yocto/exception.hpp"
#include "yocto/sequence/list.hpp"

namespace yocto
{
    namespace network
    {

        protocol:: ~protocol() throw()
        {
        }

        protocol:: protocol( const socket_address &ipaddr, const int max_pending) :
        server(ipaddr,max_pending),
        sockset(),
        conn_db(),
        cache(256),
        stand_by(1.0)
        {

            sockset.insert(server);
            
        }



        void protocol:: execute()
        {
            list<sock_key_t> to_drop;

            bool run = true;
            while(run)
            {
                //______________________________________________________________
                //
                // do we have someting to send
                //______________________________________________________________
                double          delay_value       = stand_by;
                bool            would_send        = false;
                const conn_iter conn_end          = conn_db.end();
                for( conn_iter i = conn_db.begin(); i != conn_end; ++i )
                {
                    connexion &cnx = **i;
                    if( (cnx.sending=(cnx.sendQ.bytes()>0) ) )
                    {
                        would_send  = true;
                        delay_value = 0;
                    }
                }

                //______________________________________________________________
                //
                // network scanning
                //______________________________________________________________
                delay            d = delay_value;
                const size_t     n = sockset.check(d);
                if(n>0)
                {

                    to_drop.free();
                    
                    //__________________________________________________________
                    //
                    // Check receiving, don't touch current connection
                    //__________________________________________________________
                    for( conn_iter i = conn_db.begin(); i != conn_db.end(); ++i )
                    {

                    }


                    //__________________________________________________________
                    //
                    // Check server status
                    //__________________________________________________________
                    if( sockset.is_ready(server) )
                    {
                        // a new connexion
                        conn_ptr         cnx( new connexion(server,cache) );
                        const sock_key_t cnx_key = cnx->key();
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
                            (void)conn_db.remove(cnx_key);
                            throw;
                        }
                    }


                }

            }

        }


    }

}
