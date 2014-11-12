#ifndef YOCTO_MEMORY_kARENA_INCLUDED
#define YOCTO_MEMORY_kARENA_INCLUDED 1

#include "yocto/os.hpp"

namespace yocto
{
    namespace memory
    {
        
        class kChunk;
        class kArena
        {
        public:
            kArena( const size_t block_size, const size_t chunk_size ) throw();
            ~kArena() throw();
            
            const size_t block_size; //!< data block size
            const size_t chunk_size; //!< chunk size for every chunk
            
            void *acquire();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(kArena);
            size_t  available; //! bookeeping
            kChunk *acquiring;
            kChunk *releasing;
            size_t  max_chunks;
            size_t  num_chunks;
            kChunk *chunks;
            const size_t add_chunks;
        };
        
    }
    
}

#endif
