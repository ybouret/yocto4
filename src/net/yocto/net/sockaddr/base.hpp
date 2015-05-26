#ifndef YOCTO_SOCKADDR_BASE_INCLUDED
#define YOCTO_SOCKADDR_BASE_INCLUDED 1

#include "yocto/net/types.hpp"
#include "yocto/net/sockaddr/sys.hpp"
#include "yocto/memory/buffer.hpp"
#include "yocto/string.hpp"
#include "yocto/counted.hpp"

namespace yocto
{
    namespace network
    {
        //! named host type
        enum socket_address_type
        {
            socket_address_none,       //!< not an address
            socket_address_any,        //!< any address
            socket_address_loopback    //!< special loopback address
        };

        //! forward of socket_address_format
        struct socket_address_format;

        //! IPv[4|6] address
        class socket_address : public memory::rw_buffer, public counted
        {
        public:
            const socket_address_format &fmt;       //!< external format

            virtual ~socket_address() throw();     //!< default destructor
            virtual size_t length() const throw(); //!< rw_buffer interface

        private:
            typedef sockaddr_in  type_v4;
            static const size_t  size_v4 = sizeof( type_v4 );
            typedef sockaddr_in6 type_v6;
            static const size_t  size_v6 = sizeof( type_v6 );

        public:
            static const size_t  storage = size_v4 > size_v6 ? size_v4 : size_v6;
            static const size_t  items64 = YOCTO_U64_FOR_SIZE(storage);

        protected:
            uint64_t   addr_[ items64 ];
            void      *host_;



        public:
            net16_t &port; //!< the port store in network format

            //! setup a predefined socket_address
            explicit socket_address( const socket_address_format &, socket_address_type net_type, net16_t net_port ) throw();

            //! setup a socket_address with name resolution
            /**
             * \param fmt the socket format
             * \param net_name the host name
             * \param net_port the host port (big endian)
             * if net_port is zero then try to extract host_name:port from net_name.
             */
            explicit socket_address( const socket_address_format &fmt, const string & net_name, net16_t net_port );

            //! copy constructor
            socket_address( const socket_address &sa ) throw();

            friend bool operator==( const socket_address &lhs, const socket_address &rhs ) throw();
            friend bool operator!=( const socket_address &lhs, const socket_address &rhs ) throw();
            
        private:
            YOCTO_DISABLE_ASSIGN(socket_address);
            friend class net;
            virtual const void * get_address() const throw(); //!< rw_buffer interface
            friend std::ostream & operator<<( std::ostream &, const socket_address &sa );
            
        };

    }
}


#endif

