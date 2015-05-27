#include "yocto/net/socket/set.hpp"
namespace yocto
{
    namespace network
    {

        const size_t socket_set::max_size = FD_SETSIZE;

        socket_set:: sock_cmp::  sock_cmp() throw() {}
        socket_set:: sock_cmp:: ~sock_cmp() throw() {}

        int socket_set:: sock_cmp:: operator()( const socket_ptr &lhs, const socket_ptr &rhs ) const throw()
        {
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
        
        void socket_set:: insert( const socket_ptr &s )
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
            if( !sock_reg_.insert( s ) ) {
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
        
        void socket_set:: remove( const socket_ptr &s ) throw()
        {
            (void)sock_reg_.remove( s );
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


namespace yocto
{
    namespace network
    {
        
        //TODO: thread protection
        size_t socket_set:: check( delay &d )
        {
            const size_t n = sock_reg_.size();
            if( n > 0 )
            {
                //std::cout << "[network: waiting for " << d.get() << " s]" << std::endl;
                
                //------------------------------------------------------
                // fill the sets
                //------------------------------------------------------
                fd_set *fd_recv = & recv_set_;
                fd_set *fd_send = & send_set_;
                
                FD_ZERO(fd_recv);
                FD_ZERO(fd_send);
                const socket_ptr *fd_iter = &sock_reg_[1];
                
#if defined(YOCTO_BSD)
                const int fd_max_p1 = 1 + (*fd_iter)->socket_; //!< for select
                //std::cout << "fd_max=" << fd_max_p1-1 << std::endl;
#endif
                
                YOCTO_LOOP_(n,
                            const socket_ptr    sp = *fd_iter;
                            const socket_t      fd = sp->socket_;
                            FD_SET(fd,fd_recv);     /* always check recv status          */
                            if( sp->sending )
                            FD_SET(fd,fd_send);     /* check if socket is ready to send */
                            ++fd_iter
                            );
                
                //std::cout << "#sending=" << ns << std::endl;
                //if( 0 == ns ) fd_send = NULL;
                
#if defined(YOCTO_BSD)
                int       res = 0;
                while( (res=::select( fd_max_p1, fd_recv, fd_send, NULL, d.wait_ ) ) < 0 ) {
                    const int err = errno;
                    switch( err ) {
                        case EINTR:
                            continue;
                            
                        default:
                            throw libc::exception( err, "::select" );
                    }
                }
                return res;
#endif
                
#if defined(YOCTO_WIN)
#if defined(_MSC_VER)
                // switch with only a default label
#pragma warning ( disable : 4065 )
#endif
                int res = 0;
                while( (res = ::select( 0, fd_recv, fd_send, NULL, d.wait_ ) ) == SOCKET_ERROR )
                {
                    const DWORD err = ::WSAGetLastError();
                    switch( err ) {
                        default:
                            throw win32::exception( err, "::select" );
                    }
                }
                return res;
#endif
                
            }
            else
            {
                return 0;
            }
        }
        
    }
}

