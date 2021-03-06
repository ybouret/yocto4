#ifndef YOCTO_NET_SOCKET_SET_INCLUDED
#define YOCTO_NET_SOCKET_SET_INCLUDED 1

#include "yocto/net/socket.hpp"
#include "yocto/net/delay.hpp"
#include "yocto/ordered/sorted-vector.hpp"
#include "yocto/memory/pooled.hpp"

#if defined(__MACH__) || defined(__FreeBSD__)
#include <sys/select.h>
#endif


namespace yocto
{
    namespace network
    {

        class socket_set : public container
        {
        public:
            static const size_t max_size; //!< system FD_SET max sizes
            virtual ~socket_set() throw();
            explicit socket_set();
            explicit socket_set(size_t);
            
            virtual const char *name() const throw();      //!< socket_set
            virtual size_t      size() const throw();      //!< sockets in set
            virtual size_t      capacity() const throw();  //!< current capacity
            virtual void        free() throw();            //!< reset the register
            virtual void        release() throw();         //!< free and release memory
            virtual void        reserve( size_t n );       //!< if possible
            
            void insert( socket &s );                //!< insert a socket into the set
            void remove( const socket &s ) throw();  //!< remove a socket from the set
            
            //!check the activity of the set.
            /**
             *  \param d a network delay (-1=>wait until something happens)
             *	\return the number of active sockets.
             */
            size_t check( delay &d );
            
            //! Test the activity of a socket.
            /**
             - true if the socket is active after check
             - always false after a second call!
             */
            bool        is_ready( const socket  &s ) throw();

            //! Test the availability of a socket
            /**
             - true if the socket can send after
             - always false after a second call!
             \note the socket must have sending=true before check
             */
            bool        can_send( const socket &s ) throw();
            
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
            typedef sorted_vector<socket_ptr,sock_cmp,memory::pooled::allocator> fd_reg;
            fd_reg sock_reg_; //!< user's registered sockets
            fd_set recv_set_; //!< ready to recv
            fd_set send_set_; //!< ready to send
        };
        
    }
}

#endif

