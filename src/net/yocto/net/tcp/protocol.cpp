#include "yocto/net/tcp/protocol.hpp"

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
                const conn_iter conn_end          = conn_db.end();
                double          delay_value       = stand_by;
                bool            has_data_to_send  = false;

                for( conn_iter i = conn_db.begin(); i != conn_end; ++i )
                {
                    connexion &cnx = **i;
                    if( (cnx.sending=(cnx.sendQ.bytes()>0) ) )
                    {
                        has_data_to_send = true;
                        delay_value      = 0;
                    }
                }

                //______________________________________________________________
                //
                // network scanning
                //______________________________________________________________
                delay        d = delay_value;
                const size_t n = sockset.check(d);

                if(n>0)
                {

                }

            }

        }


    }

}
