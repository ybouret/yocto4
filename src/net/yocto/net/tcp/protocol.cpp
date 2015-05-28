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
        disconnected(),
        stand_by(1.0)
        {

            sockset.insert(server);

        }



        void protocol:: execute()
        {

            bool run = true;
            while(run)
            {
                //______________________________________________________________
                //
                // do we have someting to send
                //______________________________________________________________
                double          delay_value       = stand_by;
                bool            would_send        = false;
                for( conn_iter i = conn_db.begin(); i != conn_db.end(); ++i )
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

                    check_recv();
                    check_conn();



                }

            }

        }


    }

}

namespace yocto
{
    namespace network
    {
        void protocol:: check_recv()
        {
            //__________________________________________________________________
            //
            // Check receiving, don't touch current connections
            //__________________________________________________________________
            disconnected.free();
            for( conn_iter i = conn_db.begin(); i != conn_db.end(); ++i )
            {
                connexion   &cnx = **i;
                if(!sockset.is_ready(cnx)) continue;

                const size_t nr  = cnx.enqueue_recv();
                if(nr<=0)
                {
                    //__________________________________________________________
                    //
                    // will drop
                    //__________________________________________________________
                    disconnected.push_back(cnx.key());
                }
                else
                {
                    //__________________________________________________________
                    //
                    // something has arrived
                    //__________________________________________________________
                    if(cnx.closed)
                    {
                        cnx.recvQ.free(); // drop garbage
                    }
                    else
                    {
                        //------------------------------------------------------
                        //
                        // PROCESS INPUT
                        //
                        //------------------------------------------------------

                    }
                }
            }

            //__________________________________________________________________
            //
            // drop disconnected
            //__________________________________________________________________
            while( disconnected.size() )
            {
                const sock_key_t k = disconnected.back();
                disconnected.pop_back();
                conn_ptr *pcnx = conn_db.search(k);
                if(!pcnx)
                {
                    throw exception("unexpected unregistered disconnected!");
                }
                connexion &cnx = **pcnx;

                //--------------------------------------------------------------
                //
                // PROCESS DISCONNECT
                //
                //--------------------------------------------------------------
                {
                    
                }
                
                sockset.remove(cnx);
                (void) conn_db.remove(k);
            }
            
        }
    }
}


namespace yocto
{
    namespace network
    {
        void protocol:: check_conn()
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

                try
                {
                    //--------------------------------------------------
                    //
                    // PROCESS CONNECT
                    //
                    //--------------------------------------------------
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



