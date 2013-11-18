#ifndef YOCTO_NET_IO_QUEUE_INCLUDED
#define YOCTO_NET_IO_QUEUE_INCLUDED 1


#include "yocto/net/io/cache.hpp"
#include "yocto/core/list.hpp"

#include "yocto/ios/istream.hpp"
#include "yocto/ios/ostream.hpp"

namespace yocto
{
	namespace network
	{
		
		//! buffer between network hardware and user space
		class io_queue : public ios::istream, public ios::ostream
		{
		public:
			explicit io_queue( io_cache &db );
			virtual ~io_queue() throw();
			
			
			
			//! receive data from an io_socket
			/**
			 data are stored in recv_blocks from
			 a new block. If possible, compacted with the 
			 last recv_blocks.
			 */
			bool recv( io_socket &sock );
			
			//! send data into an io_socket
			/**
			 data are taken from the first send_blocks.
			 return true if send_blocks is empty afterwards
			 */
			bool sent( io_socket &sock );
			
			void reset() throw();

			//------------------------------------------------------------------
			// use recv blocks
			//------------------------------------------------------------------
			virtual bool query( char &C );                             //!< from the head recv blocks.
			virtual void store( char  C );                             //!< into the head recv blocks
			virtual void get( void *data, size_t size, size_t &done ); //!< gather recv blocks
			void         clear_recv() throw();                         //!< no more recv blocks...
			
			//------------------------------------------------------------------
			// use send blocks
			//------------------------------------------------------------------
			virtual void write( char C );                                   //!< append at tail of send blocks
			virtual void flush();                                           //!< here: do nothing special
			virtual void put( const void *data, size_t size, size_t &done); //!< split at tail of send blocks
			bool         would_send() const throw();                        //!< true if data in send_blocks
			
		private:
			core::list_of<io_block>  send_blocks;
			core::list_of<io_block>  recv_blocks;
		public:
			const size_t             recv_length; //!< bookeeping of available #bytes

		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(io_queue);
			io_cache                &cache;
		};
		
	}
}

#endif
