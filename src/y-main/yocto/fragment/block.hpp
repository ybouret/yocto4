#ifndef YOCTO_FRAG_BLOCK_INCLUDED
#define YOCTO_FRAG_BLOCK_INCLUDED 1


#include "yocto/ios/ichannel.hpp"

namespace yocto
{
	
	namespace fragment
	{
		
		class block 
		{
		public:
			block *next;
			block *prev;
			
			block(size_t len);
			~block() throw();
			
			size_t length() const throw(); //!< last - curr
			size_t unused() const throw(); //!< final - last
			size_t offset() const throw(); //!< curr - first
			void   clear() throw();
			
			//! write in unused bytes
			/**
			 * \return the number of written bytes.
			 */
			size_t write( const void *buffer, size_t buflen ) throw();
			
			//! read in length bytes
			/**
			 *	\return the number of read bytes.
			 */
			size_t read( void *buffer, size_t buflen ) throw();
			
			//! store back after a succesfull read
			bool   back( uint8_t x ) throw();
			
			void   defrag() throw();
			bool   try_steal( block &other ) throw(); //!< ok if other.length() <= this->unused()
			
			//! read an input channel
			bool recv( ios::ichannel &input );
			
			
		private:
			const size_t   bytes; //!< memory bytes
			uint8_t       *first; //!< memory base
			const uint8_t *final; //!< first + bytes, highest invalid address
			uint8_t       *curr;  //!< current position
			uint8_t       *last;  //!< first invalid position
			YOCTO_DISABLE_COPY_AND_ASSIGN(block);
			
			static void  dispatch( void *frag_data, size_t frag_size, void * );
		};
		
	}
	
}

#endif
