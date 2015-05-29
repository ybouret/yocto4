#ifndef YOCTO_NET_TCP_PROTOCOL_INCLUDED
#define YOCTO_NET_TCP_PROTOCOL_INCLUDED 1

#include "yocto/net/tcp/connexion.hpp"
#include "yocto/net/socket/set.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/ptr/intr.hpp"
#include "yocto/sequence/list.hpp"

namespace yocto
{
    namespace network
    {

        class protocol
        {
        public:

            explicit protocol( const socket_address &ipaddr, const int max_pending=2);
            virtual ~protocol() throw();

            void execute();

            virtual void onInit(connexion &);
            virtual void onQuit(connexion &) throw();
            virtual void onRecv(connexion &);
            virtual void onSent(connexion &);
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(protocol);
            typedef intr_ptr<sock_key_t,connexion> conn_ptr;
            typedef set<sock_key_t,conn_ptr>       conn_set;
            typedef conn_set::iterator             conn_iter;
            typedef list<sock_key_t>               keys_list;

            tcp_server server;
            socket_set sockset;
            conn_set   conn_db;
            io_cache   cache;
            keys_list  disconnected;

            bool check_init();
            void check_recv();
            void check_conn();
            void check_send();

            void disconnect();
            
        public:
            double   stand_by; //!< default=1 seconds

        };


    }

}

#endif
