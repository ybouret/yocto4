#include "yocto/net/socket/set.hpp"

namespace yocto
{
    namespace network
    {

        const size_t socket_set::max_size = FD_SETSIZE;

        socket_set:: sock_cmp::  sock_cmp() throw() {}
        socket_set:: sock_cmp:: ~sock_cmp() throw() {}

        int socket_set:: sock_cmp:: operator()( const socket_ptr lhs, const socket_ptr rhs ) const throw()
        {
            assert( NULL != lhs );
            assert( NULL != rhs );
            const ptrdiff_t L = lhs->fd_value();
            const ptrdiff_t R = rhs->fd_value();
            return int(R-L);
        }
        
        
    }
}
