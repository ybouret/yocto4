#include "yocto/net/bsd/socket.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/net/sockaddr/base.hpp"
#include "yocto/net/sockaddr/format.hpp"

#if defined(YOCTO_BSD)
#include <cerrno>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#endif

#include <climits>
#include <iostream>

namespace yocto
{

    namespace network
    {
#if defined(YOCTO_WIN)
        const socket_t invalid_socket = INVALID_SOCKET;
#endif

#if defined(YOCTO_BSD)
        const socket_t invalid_socket = -1;
#endif


        //==================================================================
        // close
        //==================================================================
        void bsd::socket_close( socket_t &s ) throw()
        {
            assert( s != invalid_socket );
            std::cerr << "<close socket>" << std::endl;

#if defined(YOCTO_WIN)
            ::closesocket( s );
#endif


#if defined(YOCTO_BSD)
            YOCTO_GIANT_LOCK();
            while( ::close(s) != 0 ) {
                const int err = errno;
                switch( err ) {
                    case EINTR:
                        continue;

                    default:
                    {
                        //TODO: bad
                        goto DONE;
                    }

                }
            }
        DONE:
#endif

            s = invalid_socket;
        } // socket_close


        //==================================================================
        // shutdown
        //==================================================================
        void bsd::socket_shutdown( socket_t s, shutdown_mode sd ) throw()
        {
            assert( s != invalid_socket );
            std::cerr << "<shutdown socket>" << std::endl;
            switch( sd )
            {
                case shutdown_recv:
#if defined(YOCTO_BSD)
                    ::shutdown( s, SHUT_RD );
#endif

#if defined(YOCTO_WIN)
#if !defined(SD_RECEIVE)
#define SD_RECEIVE 0x00
#endif
                    ::shutdown( s, SD_RECEIVE);
#endif

                    break;

                case shutdown_send:
#if defined(YOCTO_BSD)
                    ::shutdown( s, SHUT_WR );
#endif

#if defined(YOCTO_WIN)
#if !defined(SD_SEND)
#define SD_SEND 0x01
#endif
                    ::shutdown( s, SD_SEND );
#endif

                    break;

                case shutdown_both:
#if defined(YOCTO_BSD)
                    ::shutdown( s, SHUT_RDWR );
#endif

#if defined(YOCTO_WIN)
#if !defined(SD_BOTH)
#define SD_BOTH 0x02
#endif
                    ::shutdown( s, SD_BOTH );
#endif

                    break;
            }
        }

        //==================================================================
        // options
        //==================================================================
        socket_t  bsd:: socket_set_default_options(socket_t s)
        {
            assert( s != invalid_socket );
            std::cerr << "<set default options>" << std::endl;

            try
            {
                const sol_socket_parameter p = { so_reuseaddr, { true } };
                set_socket_option( s, p );
                return s;
            }
            catch(...)
            {
                socket_close(s);
                throw;
            }
        }

        void     bsd:: socket_set_blocking( socket_t s, bool b )
        {
            assert( s != invalid_socket );
            const sol_socket_parameter p = { so_blocking, { b } };
            set_socket_option( s, p );
        }



        //==================================================================
        // TCP
        //==================================================================
        socket_t net:: open_tcp( int protocol, const char *level ) const {

            assert(NULL!=level);
            std::cerr << "<tcp socket," << level << ">" << std::endl;

            YOCTO_GIANT_LOCK();
#if defined(YOCTO_BSD)
            socket_t s = ::socket( protocol, SOCK_STREAM, IPPROTO_TCP );
            if( s == invalid_socket )
            {
                throw libc::exception ( errno, "::socket(tcp,%s)", level );
            }
#endif

#if defined(YOCTO_WIN)
            socket_t s = ::socket( protocol, SOCK_STREAM, IPPROTO_TCP );
            if( s == invalid_socket )
            {
                throw win32::exception( ::WSAGetLastError(), "::socket(tcp,%s)", level );
            }
#endif

            std::cerr << "<tcp socket=" << s << ">" << std::endl;
            return s;
        }

        //==================================================================
        // UDP
        //==================================================================
        socket_t net:: open_udp( int protocol, const char *level ) const
        {
            assert(NULL != level );
            std::cout << "<udp socket," << level << ">" << std::endl;
            YOCTO_GIANT_LOCK();

#if defined(YOCTO_BSD)
            socket_t s = ::socket( protocol, SOCK_DGRAM, IPPROTO_UDP );
            if( s == invalid_socket )
            {
                throw libc::exception( errno, "::socket(udp,%s)", level );
            }
#endif

#if defined(YOCTO_WIN)
            socket_t s = ::socket( protocol, SOCK_DGRAM, IPPROTO_UDP );
            if( s == invalid_socket )
            {
                throw win32::exception( ::WSAGetLastError(), "::socket(udp,%s)", level );
            }
#endif

            std::cout << "<udp socket=" << s << ">" << std::endl;
            return s;
        }






        //==================================================================
        // bind
        //==================================================================
        socket_t bsd::socket_bind( socket_t s, const socket_address &ip ) {
            assert( s != invalid_socket );
            std::cout << "<bind#" <<  swap_be(ip.port) << ">" << std::endl;
            YOCTO_GIANT_LOCK();
#if defined(YOCTO_BSD)
            while( ::bind( s, (struct sockaddr *)ip.ro(), ip.length() ) != 0 )
            {
                const int err = errno;
                switch( err )
                {
                    case EINTR:
                        continue;

                    default:
                        socket_close( s );
                        throw libc::exception(err,"::bind(%s)", ip.fmt.version );
                }
            }
#endif

#if defined(YOCTO_WIN)
#if defined(_MSC_VER)
#pragma warning ( disable : 4065 )
#endif
            while( ::bind( s, (struct sockaddr *)ip.ro(), ip.length() ) == SOCKET_ERROR )
            {
                const DWORD err = ::WSAGetLastError();
                switch( err )
                {
                    default:
                        socket_close( s );
                        throw win32::exception(err,"::bind(%s)", ip.fmt.version );
                }
            }
#endif

            return s;
        } // socket_bind

        //==================================================================
        // listen
        //==================================================================
        socket_t  bsd::socket_listen( socket_t s, int max_pending)
        {
            assert( s != invalid_socket );
            std::cout << "<listen(" << max_pending << ")>" << std::endl;
            YOCTO_GIANT_LOCK();
#if defined(YOCTO_BSD)
            if( ::listen( s, max_pending ) < 0 )
            {
                const int err = errno;
                socket_close(s);
                throw libc::exception( err, "::listen(%d)", max_pending );
            }
#endif

#if defined(YOCTO_WIN)
            if( ::listen( s, max_pending) == SOCKET_ERROR )
            {
                const DWORD err = ::WSAGetLastError();
                socket_close(s);
                throw win32::exception( err, "::listen(%d)", max_pending );
            }
#endif

            return s;
        } // listen

        //==================================================================
        // connect
        //==================================================================
        socket_t  bsd:: socket_connect( socket_t s, const socket_address &ip )
        {
            assert( s != invalid_socket );

            std::cout << "<connect@" << ip << ":" << swap_be16(ip.port) << ">" << std::endl;
            YOCTO_GIANT_LOCK();
#if defined(YOCTO_BSD)
            while( ::connect( s, (struct sockaddr *)ip.ro(), ip.length() ) != 0 )
            {
                const int err = errno;
                switch( err )
                {
                    case EINTR:
                        continue;

                    default:
                        socket_close( s );
                        throw libc::exception(err,"::connect(%s)",ip.fmt.version);
                }
            }
#endif

#if defined(YOCTO_WIN)
            while( ::connect( s, (struct sockaddr *)ip.ro(), ip.length() )  == SOCKET_ERROR )
            {
                const DWORD err = ::WSAGetLastError();
                switch( err )
                {
                    default:
                        socket_close( s );
                        throw win32::exception(err,"::connect(%s)", ip.fmt.version);
                }
            }
#endif

            return s;
        } // connect


        //==================================================================
        // accept
        //==================================================================
        socket_t bsd:: socket_accept( const socket_t s, socket_address &ip )
        {

            assert( s != invalid_socket );

            std::cout << "<accept>" << std::endl;

            YOCTO_GIANT_LOCK();
#if defined(YOCTO_BSD)
            socklen_t   tcp_client_len    = ip.length();
            socket_t    tcp_client_socket = ::accept( s, (struct sockaddr *)ip.ro(), &tcp_client_len );
            if( tcp_client_socket == invalid_socket )
            {
                throw libc::exception( errno, "::accept(%s)", ip.fmt.version );
            }
#endif

#if defined(YOCTO_WIN)
            int         tcp_client_len    =  ip.length();
            socket_t    tcp_client_socket = ::accept( s, (struct sockaddr *)ip.ro(), &tcp_client_len );
            if( tcp_client_socket == invalid_socket )
            {
                throw win32::exception( ::WSAGetLastError(), "::accept(%s)", ip.fmt.version );
            }
#endif

            return tcp_client_socket;
        }

        //==================================================================
        // send
        //==================================================================
        size_t bsd:: socket_send( socket_t s, const void *ptr, size_t len, int flags )
        {

            assert( s != invalid_socket );
            assert(!(ptr==NULL&&len>0));

            YOCTO_GIANT_LOCK();
#if defined(YOCTO_BSD)
            //std::cerr << "[send len=" << len << "]" << std::endl;
            int res=0;
            while( (res = ::send( s, ptr, len, flags ) ) < 0 ) {
                //std::cerr << "send error!" << std::endl;
                const int err = errno;
                switch( err ) {
                    case EINTR:
                        continue;

                    default:
                        throw libc::exception( err, "::send");
                }
            }
            //std::cerr << "[send res=" << res << "]" << std::endl;
            return res;
#endif

#if defined(YOCTO_WIN)
            int       res = 0;
            const int iln = min_of<size_t>(len,INT_MAX);
            while( (res = ::send( s, (const char *)ptr, iln, flags) ) == SOCKET_ERROR ) {
                const DWORD err = ::WSAGetLastError();
                throw win32::exception( err, "::send");
            }
            return res;
#endif
        } //socket_send


        //==================================================================
        // recv
        //==================================================================
        size_t bsd:: socket_recv( socket_t s, void  *ptr, size_t len, int flags)
        {
            assert( s != invalid_socket );
            assert(!(ptr==NULL&&len>0));
            YOCTO_GIANT_LOCK();
#if defined(YOCTO_BSD)
            int res=0;
            while( (res = ::recv( s, ptr, len, flags ) ) < 0 ) {
                const int err = errno;
                switch( err ) {
                    case EINTR:
                        continue;
                        
                    default:
                        throw libc::exception( err, "::recv");
                }
            }
            return res;
#endif
            
#if defined(YOCTO_WIN)
            int       res = 0;
            const int iln = min_of<size_t>(len,INT_MAX);
            while( (res = ::recv( s, (char *)ptr, iln, flags) ) == SOCKET_ERROR ) {
                const DWORD err = ::WSAGetLastError();
                throw win32::exception( err, "::recv");
            }
            return res;
#endif
        } //recv
        
        //==================================================================
        // send_to
        //==================================================================
        size_t bsd:: socket_send_to( socket_t s, const socket_address &ip, const void *ptr, size_t len, int flags)
        {
            assert(!(ptr==NULL&&len>0));
            assert( s != invalid_socket );
            YOCTO_GIANT_LOCK();
#if defined(YOCTO_BSD)
            int res=0;
            while( (res= ::sendto( s , ptr, len, flags, (const sockaddr *)(ip.ro()), ip.length() )) < 0) {
                const int err = errno;
                switch( err ) {
                    case EINTR:
                        continue;
                    default:
                        throw libc::exception( err, "::sendto(%s)", ip.fmt.version );
                }
            }
            return res;
#endif
            
#if defined(YOCTO_WIN)
            int       res = 0;
            const int iln = min_of<size_t>(len,INT_MAX);
            while( (res = ::sendto( s , (const char *)ptr, iln, flags, (const sockaddr *)(ip.ro()), ip.length() )) == SOCKET_ERROR) {
                const DWORD err = ::WSAGetLastError();
                throw win32::exception( err, "::sendto(%s)", ip.fmt.version );
                
            }
            return res;
#endif
        } // send_to
        
        size_t bsd:: socket_recv_from( socket_t s, socket_address &ip, void *ptr, size_t len, int flags)
        {
            assert(!(ptr==NULL&&len>0));
            assert( s != invalid_socket );
            
            int res = 0;
            YOCTO_GIANT_LOCK();
#if defined(YOCTO_BSD)
            socklen_t slen = ip.length();
            while( (res= ::recvfrom( s, ptr, len, flags, (struct sockaddr *)(ip.rw()), &slen )) < 0) {
                const int err = errno;
                switch( err ) {
                    case EINTR:
                        continue;
                    default:
                        throw libc::exception( err, "::recvfrom(%s)", ip.fmt.version );
                }
            }
#endif
            
#if defined(YOCTO_WIN)
            int slen = ip.length();
            while( (res= ::recvfrom( s, (char *)ptr, len, flags, (struct sockaddr *)(ip.rw()), &slen )) == SOCKET_ERROR ) {
                const int err = ::WSAGetLastError();
                throw win32::exception( err, "::recvfrom(%s)", ip.fmt.version );
            }
#endif
            
            return res;
            
        }
        
        
        
        void bsd:: socket_getpeername( socket_t s, socket_address &ip )
        {
            assert( s != invalid_socket );
            YOCTO_GIANT_LOCK();
#if defined(YOCTO_BSD)
            socklen_t peer_len = ip.length();
            if( ::getpeername( s, (struct sockaddr *)ip.ro(), &peer_len ) < 0 )
                throw libc::exception( errno, "::recvfrom(%s)", ip.fmt.version );
#endif
            
#if defined(YOCTO_WIN)
            int peer_len = ip.length();
            if( ::getpeername( s, (struct sockaddr *)ip.ro(), &peer_len ) < 0 )
                throw win32::exception( ::WSAGetLastError(), "::recvfrom(%s)", ip.fmt.version );
#endif
            
        }
        
        
        
        
    } // network
    
} // yocto
