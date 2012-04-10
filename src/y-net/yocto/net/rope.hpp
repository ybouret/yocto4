#ifndef YOCTO_NET_ROPE_INCLUDED
#define YOCTO_NET_ROPE_INCLUDED 1

#include "yocto/net/io/protocol.hpp"

namespace yocto 
{
    namespace network
    {
        //! Request Oriented Protocol Element Link
        class rope_link : public tcp_link
        {
        public:
            explicit rope_link( const connexion &cnx );
            virtual ~rope_link() throw();
            string request;
                        
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(rope_link);
        };
        
        //! ROPE Client
        typedef intrusive_ptr<socket_address,rope_link> rope_client;
        
         //! ROPE Server
        class rope_server : public server_protocol
        {
        public:
            explicit rope_server( const socket_address &ip, const size_t bs = 1024 );
            virtual ~rope_server() throw();
            
            virtual void on_init(connexion &);
            virtual void on_quit(connexion &) throw();
            virtual void on_recv(connexion &);
            
        private:
            typedef set<socket_address,rope_client> client_set;
            client_set clients;
            YOCTO_DISABLE_COPY_AND_ASSIGN(rope_server);
        };
        
        
    }
    
}


#endif
