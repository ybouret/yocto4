#ifndef YOCTO_MEMORY_CHUNK_INCLUDED
#define YOCTO_MEMORY_CHUNK_INCLUDED 1


#include "yocto/os.hpp"

namespace yocto
{
	namespace memory
	{
		
		class chunk
		{
		public:
			static const size_t num_blocks_min = 2;
			static const size_t num_blocks_max = 255;
			uint8_t       *data;           //!< first byte of data
			const uint8_t *last;           //!< first invalid byte
			uint8_t        firstAvailable; //!< data index
			uint8_t        stillAvailable; //!< bookeeping
			const uint8_t  providedNumber; //!< inital num_blocks
			
			chunk(void  *           data_entry,
				  size_t            block_size,
				  size_t            num_blocks,
				  size_t            chunk_size) throw();
			
			~chunk() throw();
			
			enum ownership
			{
				owned_by_this,
				owned_by_next,
				owned_by_prev
			};
			
			ownership whose( const void *addr ) const throw();
			
			void *acquire( size_t block_size ) throw();
			void  release( void *p, size_t block_size ) throw();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(chunk);
		};
		
	}
	
}

#endif
