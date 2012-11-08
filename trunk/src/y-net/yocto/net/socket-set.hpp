//! \file socket-set.hpp

#ifndef YOCTO_SOCKET_SET_INCLUDED
#define YOCTO_SOCKET_SET_INCLUDED 1

#include "yocto/net/socket.hpp"
#include "yocto/ordered/catalog.hpp"
#include "yocto/memory/pooled.hpp"

#if defined(YOCTO_BSD)
#include <cstdlib>
#endif

#if defined(__MACH__) || defined(__FreeBSD__)
#include <sys/select.h>
#endif

namespace yocto {
	
	namespace network {
		
		//! wrapper for select(...) timeval
		class delay : public object {
		public:
			explicit delay( const double seconds=-1.0) throw();
			virtual ~delay() throw();
			delay( const delay &) throw();
			delay&operator=( const delay & other ) throw();
			
			double operator*() const throw();
			
		private:
			struct timeval  data_; //!< time from 0 to 1 day.
			struct timeval *wait_; //!< NULL: wait for ever else wait = data
			friend class socket_set;
		};
		
		//! manage some fd_set
		class socket_set : public container
		{
		public:
			explicit socket_set() throw();
			explicit socket_set( size_t n );
			virtual ~socket_set() throw();
			
			
			virtual const char *name() const throw();
			virtual size_t      size() const throw();      //!< sockets in set
			virtual size_t      capacity() const throw();  //!< current capacity
			virtual void        free() throw();            //!< reset the register
			virtual void        release() throw();         //!< free and release memory
			virtual void        reserve( size_t n );
			
			void insert( socket & s );          //!< insert a socket into the set
			void remove( socket & s ) throw();  //!< remove a socket from the set
			
			static const size_t max_size; //!< system FD_SET max sizes
			
			
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
			bool        is_ready( const socket &s ) throw();
			
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
			
			typedef  catalog<socket_ptr,sock_cmp,memory::pooled::allocator> fd_reg; //!< keep track AND order => highest fd at front !
			fd_reg sock_reg_; //!< user's registered sockets
			fd_set recv_set_; //!< ready to recv
			fd_set send_set_; //!< ready to send
			
		};
		
	}
	
}


#endif
