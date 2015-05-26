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


#include "yocto/code/utils.hpp"
#include "yocto/exceptions.hpp"
#include <cerrno>

namespace yocto
{
    namespace network
    {
        socket_set:: ~socket_set() throw()
        {
            
        }
        
        socket_set:: socket_set() :
        sock_reg_(),
        recv_set_(),
        send_set_()
        {
            
        }
        
        

        socket_set::socket_set( size_t n ) :
        sock_reg_( min_of<size_t>(n,max_size), as_capacity ),
        recv_set_(),
        send_set_()
        {
        }
        
        
        size_t socket_set:: size() const throw() {
            return sock_reg_.size();
        }
        
        size_t socket_set:: capacity() const throw()
        {
            return min_of( max_size, sock_reg_.capacity() );
        }
        
        const char * socket_set::name() const throw() { return "socket_set"; }
        
        void socket_set:: insert( socket &s )
        {
            //----------------------------------------------------------
            // use O/S hard limit
            //----------------------------------------------------------
            if( sock_reg_.size() >= max_size )
                throw libc::exception( ERANGE, "socket_size::max_size(%d) exceeded", int(max_size));
            
            //----------------------------------------------------------
            // then register the descriptor
            //----------------------------------------------------------
            //const bsd::socket_t sock = s.socket_;
            if( !sock_reg_.insert( &s ) ) {
                throw libc::exception( EINVAL, "socket already in socket_set" );
            }
        }
        
        void socket_set:: reserve( size_t n )
        {
            if( n + capacity() >= max_size )
            {
                throw libc::exception(ERANGE,
                                      "socket_size.reserve(%u) from capacity=%u exceeds max_size(%d)",
                                      unsigned(n),
                                      unsigned( capacity() ),
                                      int(max_size) );
            }
            else {
                sock_reg_.reserve( n );
            }
            
            
        }
        
        void socket_set:: remove( socket &s ) throw() 
        {
            (void)sock_reg_.remove( &s );
        }
        
        void socket_set:: free() throw()
        {
            sock_reg_.free();
        }
        
        
        void socket_set:: release() throw()
        {
            sock_reg_.release();
        }
        
        
    }

}

