#ifndef YOCTO_MEMORY_CHUNK_INCLUDED
#define YOCTO_MEMORY_CHUNK_INCLUDED 1


#include "yocto/os.hpp"

namespace yocto
{
	namespace memory
	{
		//! a chunk of blocks
		/**
		 * should have a 2*sizeof(void *) size.
		 */
		class chunk
		{
		public:
			static const size_t num_blocks_min = 2;
			static const size_t num_blocks_max = 255;
			
			uint8_t       *data;           //!< first byte of data
			uint8_t        firstAvailable; //!< data index
			uint8_t        stillAvailable; //!< bookeeping
			const uint8_t  providedNumber; //!< inital num_blocks
			uint8_t        padding[sizeof(uint8_t*)-3];
			
			//! prepare a chunk
			/**
			 \param data_entry a valid memory area >= chunk_size bytes
			 \param block_size the block size
			 \param num_blocks initially available #blocks
			 \param chunk_size at least block_size * num_blocks
			 */
			chunk(void  *           data_entry,
				  size_t            block_size,
				  size_t            num_blocks,
				  size_t            chunk_size) throw();
			
			inline ~chunk() throw() {}
			
			enum ownership
			{
				owned_by_this,
				owned_by_next,
				owned_by_prev
			};
			
			ownership whose( const void *addr, const size_t chunk_size ) const throw();
			
			void *acquire( size_t block_size ) throw();
			void  release( void *p, size_t block_size ) throw();
			
			bool is_empty() const throw();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(chunk);
		};
		
	}
	
}

#endif
