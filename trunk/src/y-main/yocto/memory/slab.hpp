#ifndef YOCTO_MEMORY_SLAB_INCLUDED
#define YOCTO_MEMORY_SLAB_INCLUDED 1


#include "yocto/code/ilog2.hpp"

namespace yocto
{
	
	namespace memory 
	{
		
		class slab 
		{
		public:
			static const size_t bs_round_ln2   = ilog2<sizeof(size_t)>::value;
			
			static size_t bytes_for( size_t & block_size, size_t num_blocks );
			static size_t round_bs( size_t block_size ) throw();
			
			~slab() throw();
			slab( void *entry, const size_t block_size, const size_t num_blocks ) throw();
			
			void *acquire() throw();
			void  release(void *) throw();
			
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(slab);
			uint8_t      *data;
			size_t        firstAvailable; //!< data index
			size_t        stillAvailable; //!< bookeeping
			
		public:
			const size_t  blockSize;
			const size_t  numBlocks; //!< inital num_blocks
			
		};
		
	}
	
}

#endif
