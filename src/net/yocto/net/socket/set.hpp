#ifndef YOCTO_NET_SOCKET_SET_INCLUDED
#define YOCTO_NET_SOCKET_SET_INCLUDED 1

#include "yocto/net/socket.hpp"
#include "yocto/net/delay.hpp"


#if defined(__MACH__) || defined(__FreeBSD__)
#include <sys/select.h>
#endif


namespace yocto
{
    namespace network
    {

        class socket_set
        {
        public:
            static const size_t max_size; //!< system FD_SET max sizes
            

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(socket_set);
            typedef socket *socket_ptr;

            //! order socket by DECREASING file descriptor value
            class    sock_cmp
            {
            public:
                sock_cmp() throw();
                ~sock_cmp() throw();
                int operator()( const socket_ptr lhs, const socket_ptr rhs ) const throw();
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(sock_cmp);
            };

        };

    }
}

#endif

