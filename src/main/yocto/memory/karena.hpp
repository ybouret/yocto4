#ifndef YOCTO_MEMORY_kARENA_INCLUDED
#define YOCTO_MEMORY_kARENA_INCLUDED 1

#include "yocto/memory/kchunk.hpp"

namespace yocto
{
    namespace memory
    {
        
        class kArena
        {
        public:
            kArena( const size_t block_size, const size_t chunk_size ) throw();
            ~kArena() throw();
            
            const size_t block_size; //!< data block size
            const size_t chunk_size; //!< chunk size for every chunk
            
            void *acquire();
            void  release(void *p) throw();
            
            enum ownership
            {
                prevChunk,
                selfChunk,
                nextChunk
            };
            
            ownership is_owner(const kChunk *ch, const void *addr) const throw();
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(kArena);
            size_t  available;       //!< bookeeping
            kChunk *acquiring;       //!< acquiring cache
            kChunk *releasing;       //!< releasing cache
            kChunk *lastEmpty;       //!< last Empty chunk
            size_t  max_chunks;      //!< max chunks for memory
            size_t  num_chunks;      //!< current #active chunks
            kChunk *chunks;          //!< linear memory of chunks
            const size_t add_chunks; //!< increase of #chunks per re-alloc
        };
        
    }
    
}

#endif
