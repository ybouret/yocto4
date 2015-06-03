#include "yocto/net/tcp/server-protocol.hpp"


namespace yocto
{
    namespace network
    {
        server_protocol:: ~server_protocol() throw()
        {
            sockset.release();
        }

        server_protocol:: server_protocol(const socket_address &ipaddr, const int max_pending, const size_t block_size ) :
        tcp_protocol(block_size),
        server(ipaddr,max_pending)
        {
            sockset.insert(server);
        }

      
    }
}


namespace yocto
{
    namespace network
    {
        void server_protocol:: execute()
        {
            if(running) return;

            running = true;
            try
            {
                while(true)
                {
                    //__________________________________________________________
                    //
                    // do we have someting to send
                    //__________________________________________________________
                    double          delay_value       = every;
                    if( initialize() )
                    {
                        delay_value = 0;
                    }
                    else
                    {
                        if(!running)
                        {
                            return;
                        }
                    }

                    //__________________________________________________________
                    //
                    // network scanning
                    //__________________________________________________________
                    delay            d = delay_value;
                    const size_t     n = sockset.check(d);
                    std::cerr << "[protocol] " << server.self() << ": #activity=" << n << std::endl;
                    if(n>0)
                    {
                        check_recv();
                        check_conn();
                        check_send();
                    }

                    //__________________________________________________________
                    //
                    // other things to do
                    //__________________________________________________________
                    onIdle();
                }
            }
            catch(...)
            {
                running = false;
                throw;
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
                enqueue(new connexion(server,cache));
            }
        }
        
    }
    
}


