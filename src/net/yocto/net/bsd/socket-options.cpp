#include "yocto/net/bsd/socket.hpp"
#include "yocto/exceptions.hpp"


#if defined(YOCTO_BSD)
#include <sys/socket.h>
#include <cerrno>
#include <fcntl.h>

#define YOCTO_SOCKOPT_BOOL  int
#define YOCTO_SOCKOPT_TRUE  1
#define YOCTO_SOCKOPT_FALSE 0
#define YOCTO_SOCKLEN_TYPE  socklen_t

#define YOCTO_SOCKOPT_INT int

#endif

#if defined(YOCTO_WIN)

#define YOCTO_SOCKOPT_BOOL  BOOL
#define YOCTO_SOCKOPT_TRUE  TRUE
#define YOCTO_SOCKOPT_FALSE FALSE
#define YOCTO_SOCKLEN_TYPE  int
#define YOCTO_SOCKOPT_INT int

#endif

#define YOCTO_SET_SOL_SOCKET_REQUEST(NAME,DATA) { NAME, #NAME, (const void *)(&DATA), sizeof(DATA) }
#define YOCTO_GET_SOL_SOCKET_REQUEST(NAME,DATA) { NAME, #NAME,       (void *)(&DATA), sizeof(DATA) }

namespace yocto {

    namespace network
    {


        //==========================================================
        //
        // SET OPTIONS
        //
        //==========================================================


        //----------------------------------------------------------
        // ENTER: level=SOL_SOCKET
        //----------------------------------------------------------
        //! SOL_SOCKET SET request: format and execution
        struct set_sol_socket_request
        {
            int                      name; //!< system
            const char *             info; //!< stringified
            const void *             args; //!< address of argument
            YOCTO_SOCKLEN_TYPE       size; //!< size of argument

            inline void set( socket_t s ) const
            {
#if defined(YOCTO_BSD)
                if( ::setsockopt( s, SOL_SOCKET, name, args, size ) != 0 )
                {
                    throw libc::exception( errno, "::setsockopt(%s)", info);
                }
#endif

#if defined(YOCTO_WIN)
                if( ::setsockopt( s, SOL_SOCKET, name, (const char *)args, size) == SOCKET_ERROR )
                {
                    throw win32::exception( :: WSAGetLastError(), "::setsockopt(%s)", info);
                }
#endif
            }
        };


#define YOCTO_SET_SOL_SOCKET_BOOLEAN(name,NAME)                                                              \
case so_##name: {                                                                                            \
const YOCTO_SOCKOPT_BOOL     yes = (param.name##_value) ? YOCTO_SOCKOPT_TRUE : YOCTO_SOCKOPT_FALSE; \
const set_sol_socket_request req = YOCTO_SET_SOL_SOCKET_REQUEST(SO_##NAME,yes);                          \
req.set(s); }                                                                                            \
break

        //!  SOL_SOCKET SET: prepare and execute request
        void  bsd:: set_socket_option( socket_t s, const sol_socket_parameter &param )
        {

            switch( param.name )
            {
                    YOCTO_SET_SOL_SOCKET_BOOLEAN(broadcast,BROADCAST);
                    YOCTO_SET_SOL_SOCKET_BOOLEAN(keepalive,KEEPALIVE);
                    YOCTO_SET_SOL_SOCKET_BOOLEAN(dontroute,DONTROUTE);
                    YOCTO_SET_SOL_SOCKET_BOOLEAN(reuseaddr,REUSEADDR);

                case so_blocking:
                {
#if defined(YOCTO_BSD)
                    int flags = 0;
                    fcntl(s, F_GETFL, &flags);
                    if( param.blocking_value )
                        flags |= O_NONBLOCK;
                    else
                        flags &= ~O_NONBLOCK;
                    fcntl(s, F_SETFL, &flags);
#endif

#if defined(YOCTO_WIN)
                    //-------------------------
                    // Set the socket I/O mode: In this case FIONBIO
                    // enables or disables the blocking mode for the
                    // socket based on the numerical value of iMode.
                    // If iMode = 0, blocking is enabled;
                    // If iMode != 0, non-blocking mode is enabled.
                    //-------------------------
                    u_long iMode = param.blocking_value ? 0 : 1;
                    ioctlsocket(s, FIONBIO, &iMode);
#endif
                }
                    break;

                default:
                    throw exception("SOL_SOCKET set option name not supported");
            }

        }

        //----------------------------------------------------------
        // LEAVE: level=SOL_SOCKET
        //----------------------------------------------------------





        //==========================================================
        //
        // GET OPTIONS
        //
        //==========================================================


        //----------------------------------------------------------
        // ENTER: level=SOL_SOCKET
        //----------------------------------------------------------
        //! SOL_SOCKET GET request: format and execute
        struct get_sol_socket_request
        {
            int                      name; //!< system
            const char *             info; //!< stringified
            void *                   args; //!< address of argument
            YOCTO_SOCKLEN_TYPE       size; //!< size of argument

            inline void get( socket_t s )
            {
#if defined(YOCTO_BSD)
                if( ::getsockopt( s, SOL_SOCKET, name, args, &size ) != 0 )
                {
                    throw libc::exception( errno, "::getsockopt(%s)", info);
                }
#endif

#if defined(YOCTO_WIN)
                if( ::getsockopt( s, SOL_SOCKET, name, (char *)args, &size) == SOCKET_ERROR )
                {
                    throw win32::exception( :: WSAGetLastError(), "::getsockopt(%s)", info);
                }
#endif
            }
        };


#define YOCTO_GET_SOL_SOCKET_BOOLEAN(name,NAME)                               \
case so_##name: {                                                             \
YOCTO_SOCKOPT_BOOL     yes = YOCTO_SOCKOPT_FALSE;                         \
get_sol_socket_request req = YOCTO_GET_SOL_SOCKET_REQUEST(SO_##NAME,yes); \
req.get(s);                                                               \
param.name##_value = ( yes == YOCTO_SOCKOPT_TRUE );                       \
}                                                                         \
break

#define YOCTO_GET_SOL_SOCKET_INTEGER(name,NAME)                              \
case so_##name: {                                                            \
YOCTO_SOCKOPT_INT      len = 0;                                          \
get_sol_socket_request req = YOCTO_GET_SOL_SOCKET_REQUEST(SO_##NAME,len);\
req.get(s);                                                              \
param.name##_value = len;                                                \
}                                                                        \
break
        
        void bsd:: get_socket_option( socket_t s, sol_socket_parameter &param )
        {
            switch( param.name )
            {
                    YOCTO_GET_SOL_SOCKET_BOOLEAN(broadcast,BROADCAST);
                    YOCTO_GET_SOL_SOCKET_BOOLEAN(keepalive,KEEPALIVE);
                    YOCTO_GET_SOL_SOCKET_BOOLEAN(dontroute,DONTROUTE);
                    YOCTO_GET_SOL_SOCKET_BOOLEAN(reuseaddr,REUSEADDR);
                    YOCTO_GET_SOL_SOCKET_INTEGER(sndbuf,SNDBUF);
                    YOCTO_GET_SOL_SOCKET_INTEGER(rcvbuf,RCVBUF);
                    
                default:
                    throw exception("SOL_SOCKET get option name not supported");
            }
        }
        
        //----------------------------------------------------------
        // LEAVE: level=SOL_SOCKET
        //----------------------------------------------------------
        
        
        
    }
    
}
