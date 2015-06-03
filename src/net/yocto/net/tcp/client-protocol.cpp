#include "yocto/net/tcp/client-protocol.hpp"

namespace yocto
{
    namespace network
    {

        client_protocol:: ~client_protocol() throw()
        {
        }


        client_protocol:: client_protocol(const size_t block_size) :
        tcp_protocol(block_size)
        {
        }




    }

}

namespace yocto
{
    namespace network
    {

        void client_protocol:: start( const socket_address &ipaddr)
        {
            if(running) return;

            running = true;
            try
            {
                enqueue( new connexion(ipaddr,cache) );
                while(1)
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
                        // nothing to send...
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
                    std::cerr << "[protocol] #activity=" << n << std::endl;
                    if(n>0)
                    {
                        check_recv();
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

