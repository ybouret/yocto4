#ifndef YOCTO_MEMORY_BLOCKS_INCLUDED
#define YOCTO_MEMORY_BLOCKS_INCLUDED 1

#include "yocto/os.hpp"

namespace yocto 
{
	namespace memory
	{
		class arena;
		class blocks 
		{
		public:
			explicit blocks( size_t chunk_size ) throw();
			virtual ~blocks() throw();
			
			void *acquire( size_t block_size );
			void  release( void *p, size_t block_size ) throw();
			
			static const size_t initial_arenas = 8;
			
			//! valid until next acquire/ensure
			arena *ensure( size_t block_size );
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(blocks);
			
			arena *cache_;            //!< caching
			arena *arena_;            //!< array of arenas
			size_t count_;            //!< #active arena
			size_t total_;            //!< max #arena in array
			arena *guard_;            //!< arena_ + count_
			const size_t chunk_size_; //!< maximum chunk size/arena
			
			arena *search( size_t block_size) throw();
		};
	}
	
}

#endif
