#include "yocto/net/tcp/protocol.hpp"
#include "yocto/exception.hpp"
#include "yocto/sequence/list.hpp"

namespace yocto
{
    namespace network
    {

        tcp_protocol:: ~tcp_protocol() throw()
        {
        }

        tcp_protocol:: tcp_protocol(size_t block_size) :
        sockset(4),
        conn_db(4,as_capacity),
        dropped(4,as_capacity),
        cache(block_size),
        every(1.0)
        {
        }
        
        void tcp_protocol:: onQuit( connexion & ) throw()
        {

        }

        void tcp_protocol:: onRecv( connexion & )
        {

        }

        void tcp_protocol:: onInit(connexion &)
        {

        }

        void tcp_protocol:: onSent(connexion &)
        {

        }


        void tcp_protocol:: onIdle()
        {
        }
        

        
    }

}



namespace yocto
{
    namespace network
    {



        void tcp_protocol:: disconnect()
        {
            //__________________________________________________________________
            //
            // drop disconnected
            //__________________________________________________________________
            while( dropped.size() )
            {
                const sock_key_t k = dropped.back();
                dropped.pop_back();
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
                    std::cerr << "disconnecting " << cnx.self() << ":" << int(swap_be(cnx.self().port)) << std::endl;
                    onQuit(cnx);
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
        bool tcp_protocol:: initialize() throw()
        {
            bool            would_send = false;
            const conn_iter end        = conn_db.end();

            for( conn_iter i = conn_db.begin(); i != end; ++i )
            {
                connexion &cnx = **i;
                if( (cnx.sending=(cnx.sendQ.bytes()>0) ) )
                {
                    would_send  = true;
                }
            }

            return would_send;
        }
    }

}


namespace yocto
{
    namespace network
    {

        void tcp_protocol:: check_recv()
        {
            //__________________________________________________________________
            //
            // Check receiving, don't touch current connections
            //__________________________________________________________________
            dropped.free();
            for( conn_iter i = conn_db.begin(); i != conn_db.end(); ++i )
            {
                connexion   &cnx = **i;
                if(!sockset.is_ready(cnx)) continue;

                const size_t nr  = cnx.do_recv();
                if(nr<=0)
                {
                    //__________________________________________________________
                    //
                    // will drop
                    //__________________________________________________________
                    dropped.push_back(cnx.key());
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
                        std::cerr << "input from " << cnx.self() << ":" << int(swap_be(cnx.self().port)) << ": #" << nr << std::endl;
                        onRecv(cnx);
                    }
                }
            }
            disconnect();

        }
    }

}
namespace yocto
{
    namespace network
    {

        void tcp_protocol:: check_send()
        {
            dropped.free();

            for( conn_iter i = conn_db.begin(); i != conn_db.end(); ++i )
            {
                connexion   &cnx = **i;
                if(cnx.sending)
                {
                    assert(cnx.sendQ.bytes()>0);

                    if(!sockset.can_send(cnx)) continue; // not ready
                    if(cnx.do_send()<=0)       continue; // no transmitted bytes
                    if(cnx.sendQ.bytes()>0)    continue; // remaining data

                    if(cnx.closed)
                    {
                        dropped.push_back(cnx.key());
                        continue;
                    }

                    //----------------------------------------------------------
                    //
                    // PROCESS SENT
                    //
                    //----------------------------------------------------------
                    onSent(cnx);
                }
            }
            
            disconnect();

        }

    }
}


#if 0
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


        bool protocol:: check_init()
        {
            bool would_send = false;
            
            for( conn_iter i = conn_db.begin(); i != conn_db.end(); ++i )
            {
                connexion &cnx = **i;
                if( (cnx.sending=(cnx.sendQ.bytes()>0) ) )
                {
                    would_send  = true;
                }
            }
            
            return would_send;
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
                if( check_init() )
                {
                    delay_value = 0;
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

            }

        }


    }

}

namespace yocto
{
    namespace network
    {
        void protocol:: disconnect()
        {
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
                    std::cerr << "disconnecting " << cnx.self() << ":" << int(swap_be(cnx.self().port)) << std::endl;
                    onQuit(cnx);
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

                const size_t nr  = cnx.do_recv();
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
                        std::cerr << "input from " << cnx.self() << ":" << int(swap_be(cnx.self().port)) << ": #" << nr << std::endl;
                        onRecv(cnx);
                    }
                }
            }
            disconnect();

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


namespace yocto
{
    namespace network
    {
        
        void protocol:: check_send()
        {
            disconnected.free();

            for( conn_iter i = conn_db.begin(); i != conn_db.end(); ++i )
            {
                connexion   &cnx = **i;
                if(cnx.sending)
                {
                    assert(cnx.sendQ.bytes()>0);

                    if(!sockset.can_send(cnx)) continue; // not ready
                    if(cnx.do_send()<=0)       continue; // no transmitted bytes
                    if(cnx.sendQ.bytes()>0)    continue; // remaining data

                    if(cnx.closed)
                    {
                        disconnected.push_back(cnx.key());
                        continue;
                    }

                    //----------------------------------------------------------
                    //
                    // PROCESS SENT
                    //
                    //----------------------------------------------------------
                    onSent(cnx);
                }
            }

            disconnect();
        }
        
    }
    
}


namespace yocto
{
    namespace network
    {
        
        void protocol:: onInit(connexion &)
        {
            
        }

        void protocol:: onQuit(connexion &) throw()
        {

        }

        void protocol:: onRecv(connexion &)
        {
        }

        void protocol:: onSent(connexion &)
        {
        }

    }
    
}
#endif



