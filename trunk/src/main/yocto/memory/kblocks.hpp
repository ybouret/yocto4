#ifndef YOCTO_MEMORY_KBLOCKS_INCLUDED
#define YOCTO_MEMORY_KBLOCKS_INCLUDED 1

#include "yocto/os.hpp"

namespace yocto
{
    namespace memory
    {
        
        class kArena;
        
        class kBlocks
        {
        public:
            static const size_t MinArenas = 8;
            
            kBlocks(const size_t a_chunk_size) throw();
            ~kBlocks() throw();
            
            const size_t chunk_size;
            
            void *acquire(const size_t block_size);
            void  release(void *addr, const size_t block_size) throw();
            
        private:
            kArena *acquiring;
            kArena *releasing;
            kArena *arenas;
            size_t  num_arenas;
            size_t  max_arenas;
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(kBlocks);
            
            
            kArena *search(const size_t block_size)  throw();
            
            
        };
        
    }
}

#endif
