#ifndef YOCTO_NET_IO_BLOCK_INCLUDED
#define YOCTO_NET_IO_BLOCK_INCLUDED 1

#include "yocto/net/io-socket.hpp"
#include "yocto/memory/buffer.hpp"

namespace yocto
{
	
	namespace network
	{
	
		class io_queue;
        
        //! block with a fixed maximal length
		class io_block : public memory::rw_buffer
		{
		public:
			
					
			void           clear() throw();
			virtual size_t length() const throw(); //!< last-curr
			size_t         unused() const throw(); //!< final - last
			size_t         offset() const throw(); //!< curr - start
			
			virtual ~io_block() throw();
			explicit io_block( size_t bs );
		
			//! return what was received
			size_t recv( io_socket &sock );
		
			//! return true if all content is sent
			bool sent( io_socket &sock );
			
			//! defrag data
			void defrag() throw();

			//! collapse blocks if possible
			bool try_steal( io_block &blk ) throw();
			
			size_t append( const void *data, size_t size ) throw();
			
		protected:
			uint8_t       *curr;  //!< current available first byte
			uint8_t       *last;  //!< first invalid byte
			uint8_t       *start; //!< where memory is allocated
			const uint8_t *final; //!< where memory ends...
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(io_block);
			virtual const void * get_address() const throw();
			friend class network::io_queue;
			
		public:
			io_block    *next; //!< for list/pool
			io_block    *prev; //!< for list
		};
		
		
	}
	
}

#endif
