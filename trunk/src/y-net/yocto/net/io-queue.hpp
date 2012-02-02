#ifndef YOCTO_NET_IO_QUEUE_INCLUDED
#define YOCTO_NET_IO_QUEUE_INCLUDED 1


#include "yocto/net/io-block.hpp"
#include "yocto/core/list.hpp"
#include "yocto/core/pool.hpp"

#include "yocto/ios/istream.hpp"
#include "yocto/ios/ostream.hpp"

namespace yocto
{
	namespace network
	{
		
		class io_queue : public ios::istream, public ios::ostream
		{
		public:
			explicit io_queue( size_t bs );
			virtual ~io_queue() throw();
			
			const size_t block_size;
			
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
			virtual bool query( char &C );
			virtual void store( char  C );
			virtual void get( void *data, size_t size, size_t &done );
			
			//------------------------------------------------------------------
			// use send blocks
			//------------------------------------------------------------------
			virtual void write( char C );
			virtual void flush();
			virtual void put( const void *data, size_t size, size_t &done);
			
			size_t send_size() const throw();
			size_t recv_size() const throw();
			size_t pool_size() const throw();
			
		private:
			core::list_of<io_block>  send_blocks;
			core::list_of<io_block>  recv_blocks;
			core::pool_of<io_block>  pool_blocks;
			io_block *fetch();
			YOCTO_DISABLE_COPY_AND_ASSIGN(io_queue);
			
		public:
			io_queue *next;
			io_queue *prev;
		};
		
	}
}

#endif
