#ifndef YOCTO_MEMORY_KCHUNK_INCLUDED
#define YOCTO_MEMORY_KCHUNK_INCLUDED 1

#include "yocto/os.hpp"

namespace yocto
{
    namespace memory
    {
        
        class kChunk
        {
        public:
            
            uint16_t          *data;            //!< first byte of data
            uint16_t           firstAvailable;  //!< bookeeping
            uint16_t           stillAvailable;  //!< bookeeping
            const uint16_t     blockIncrement;  //!< for
            
            
            //! prepare a chunk
            /**
             \param data_entry an area of num_blocks * block_size bytes
             \param block_size > 0
             \param chunk_size available data size
             the maximum number of blocks is computed
             */
            kChunk(void        *data_entry,
                   const size_t block_size,
                   const size_t chunk_size) throw();
            
            ~kChunk() throw();
            
            //! acquire memory for block_size
            void *acquire(const size_t block_size) throw();
            
            //! release memory for block_size
            void release(void *p) throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(kChunk);
        };
        
    }
}

#endif
