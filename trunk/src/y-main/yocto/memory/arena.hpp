#ifndef YOCTO_MEMORY_ARENA_INCLUDED
#define YOCTO_MEMORY_ARENA_INCLUDED 1

#include "yocto/memory/chunk.hpp"

namespace yocto
{
	
	namespace memory
	{
		
		class arena 
		{
		public:
			arena( size_t block_size, size_t num_blocks ) throw();
			~arena() throw();
			
			void *acquire_block();
			void  release_block( void * ) throw();
			
			static const size_t initial_chunks = (256/sizeof(chunk));
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(arena);
			chunk       *chunk_;      //!< array of chunks
			size_t       size_;       //!< current #chunks in array
			size_t       maxi_;       //!< maximum #chunks in array
			size_t       accessible_; //!< current accessible blocks
			chunk       *pAcquiring_; //!< never NULL once accessible_ > 0
			chunk       *pReleasing_; //!< never NULL once accessible_ > 0
			chunk       *chunk_last_; //!< first invalid chunk
			chunk       *pAvailable_; //!< an empty chunk
			const size_t block_size_; //!< the common block size
			const size_t num_blocks_; //!< num_blocks per new chunk
			const size_t chunk_size_; //!< new chunk size
			const size_t new_blocks_; //!< num_blocks_ - 1 after a new chunk
			
		};
		
	}
	
}


#endif
