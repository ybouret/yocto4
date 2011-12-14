#ifndef YOCTO_MEMORY_EMBED_INCLUDED
#define YOCTO_MEMORY_EMBED_INCLUDED 1

#include "yocto/memory/allocator.hpp"


namespace yocto
{
	namespace memory
	{
		
		struct embed
		{
			void **hook;   //!< pointer to the address
			size_t length; //!< bytes to allocate
			size_t offset; //!< offset
			
			static size_t prepare( embed reg[], size_t num ) throw();
			static void   dispatch( embed reg[], size_t num, void *workspace ) throw();
			static void  *build_with( allocator &alloc, embed reg[], size_t num, size_t &bytes );
		};
		
#define YOCTO_EMBED(PTR,NUM) { (void **)(void*)&(PTR), NUM * sizeof( *PTR ), 0 }
		
#define YOCTO_EMBED_START() do { yocto::memory::embed __em_reg[] = 
		
#define YOCTO_EMBED_ALLOC(ALLOCATOR,WORKSPACE,BYTES) ;      \
const size_t __em_num = sizeof(__em_reg)/sizeof(__em_reg[0]);\
WORKSPACE = yocto::memory::embed::build_with( ALLOCATOR,__em_reg,__em_num, BYTES); } while(false)
		
	}
}


#endif
