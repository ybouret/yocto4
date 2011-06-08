#ifndef YOCTO_MEMORY_BLOCKS_INCLUDED
#define YOCTO_MEMORY_BLOCKS_INCLUDED 1

#include "yocto/memory/arena.hpp"

namespace yocto 
{
	namespace 
	{
		class blocks 
		{
		public:
			explicit blocks() throw();
			virtual ~blocks() throw();
			
		private:
			arena *arena_;      //!< array of arenas
			size_t count_;      //!< #active arena
			size_t total_;      //!< max #arena in array
			arena *arena_last_; 
			
			YOCTO_DISABLE_COPY_AND_ASSIGN(blocks);
										
		};
	}
	
}

#endif
