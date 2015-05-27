#ifndef YOCTO_NET_SOCKET_TCP_INCLUDED
#define YOCTO_NET_SOCKET_TCP_INCLUDED 1

#include "yocto/net/socket.hpp"
#include "yocto/net/socket/io.hpp"

namespace yocto
{
    namespace network
    {
        //! TCP server
        class tcp_server : public socket
        {
        public:
            //! constructor.
            /**
             *  \param ip address to bind.
             *  \param max_pending BSD pending connections.
             */
            explicit tcp_server(const socket_address &ip,
                                const int             max_pending);

            //! default destructor.
            virtual ~tcp_server() throw();

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(tcp_server);
            friend class tcp_client;
            socket_t get_socket() const throw();
        };


        //! TCP client
        class tcp_client: public socket, public io_socket
        {
        public:
            //! connect(...) constructor
            /**
             * \param to_server where to connect
             */
            explicit tcp_client( const socket_address &to_server );

            //! destructor
            virtual ~tcp_client() throw();

            //! accept(...) constructor
            /**
             * \param srv a TCP server ready to accept() a new connection.
             */
            explicit tcp_client( tcp_server &srv );    //!< accept.

            virtual size_t send( const void *ptr, size_t len ); //!< BSD send to server
            virtual size_t recv( void       *ptr, size_t len ); //!< BSD recv from server
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(tcp_client);
        };

    }
}

#endif

