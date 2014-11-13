#if 0
#include "yocto/memory/kchunk.hpp"
#include "yocto/code/round.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/type/ints.hpp"

#include <cstring>

#include <iostream>

namespace yocto
{
    namespace memory
    {
        
        kChunk::kChunk(void        *data_entry,
                       const size_t block_size,
                       const size_t chunk_size) throw() :
        data( static_cast<word_type *>(data_entry) ),
        firstAvailable(0),
        stillAvailable(0),
        blockIncrement( YOCTO_ALIGN_FOR_ITEM(word_type,block_size) )
        {
            assert(block_size>0);
            
            
            //__________________________________________________________________
            //
            // compute all parameters
            //__________________________________________________________________
            const size_t block_round    = blockIncrement;
            const size_t max_blocks     = limit_of<word_type>::maximum;
            const size_t top_blocks     = chunk_size/block_round;
            stillAvailable              = min_of(max_blocks,top_blocks);
            (word_type &)blockIncrement = block_round/sizeof(word_type);
            
#if 0
            std::cerr << "-- max_blocks  = " << size_t(max_blocks)     << std::endl;
            std::cerr << "-- top_blocks  = " << size_t(top_blocks)     << std::endl;
            std::cerr << "-- block_size  = " << size_t(block_size)     << std::endl;
            std::cerr << "-- block_round = " << size_t(block_round)    << std::endl;
            std::cerr << "-- chunk_size  = " << size_t(chunk_size)     << std::endl;
            std::cerr << "-- num_blocks  = " << size_t(stillAvailable) << std::endl;
            std::cerr << "-- block_incr  = " << size_t(blockIncrement) << std::endl;
#endif
            
            //__________________________________________________________________
            //
            // format the chunk
            //__________________________________________________________________
            word_type *q = data;
            for( size_t i=0; i != stillAvailable; q += blockIncrement )
            {
                *q = ++i;
            }
            
        }
        
        kChunk:: ~kChunk() throw()
        {
        }
        
        void * kChunk:: acquire() throw()
        {
            assert(stillAvailable>0);
            word_type     *p = &data[firstAvailable*blockIncrement];
            firstAvailable  = *p;
            
            --stillAvailable;
            
            word_type *q = p;
            for(size_t i=blockIncrement;i>0;--i) *(q++) = 0;
            return p;
        }
        
        void kChunk:: release( void *addr ) throw()
        {
            assert( addr != NULL  );

            word_type *to_release = (word_type *)addr;
            
            assert( static_cast<ptrdiff_t>(to_release - data) % blockIncrement == 0 );
            
            /** update linking **/
            *to_release    = firstAvailable;
            firstAvailable = static_cast<size_t>( (to_release-data) / blockIncrement );
            
            assert( firstAvailable == (to_release-data) / blockIncrement );
            
            /** update status **/
            ++stillAvailable;
        }

        
        
    }
}
#endif

