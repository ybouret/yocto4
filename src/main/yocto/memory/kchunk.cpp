#include "yocto/memory/kchunk.hpp"
#include "yocto/code/round.hpp"
#include "yocto/code/utils.hpp"
#include <cstring>

#include <iostream>

namespace yocto
{
    namespace memory
    {
        
        kChunk::kChunk(void        *data_entry,
                       const size_t block_size,
                       const size_t chunk_size) throw() :
        data( static_cast<uint16_t *>(data_entry) ),
        firstAvailable(0),
        stillAvailable(0),
        blockIncrement( YOCTO_ROUND2(block_size) )
        {
            assert(block_size>0);
            
            const size_t block_round = blockIncrement;
            //__________________________________________________________________
            //
            // compute all parameters, using blockIncrement as block_round
            //__________________________________________________________________
            std::cerr << "\tblock_size  = " << block_size  << std::endl;
            std::cerr << "\tblock_round = " << block_round << std::endl;
            (uint16_t &)stillAvailable = min_of<size_t>(65535,chunk_size/block_round);
            std::cerr << "\tnum_blocks  = " << stillAvailable << std::endl;
            
            (uint16_t &)blockIncrement = block_round>>1;
            std::cerr << "\tblock_incr  = " << blockIncrement << std::endl;
            
            //__________________________________________________________________
            //
            // format the chunk
            //__________________________________________________________________
            uint16_t *q = data;
            for( uint16_t i=0; i != stillAvailable; q += blockIncrement )
            {
                *q = ++i;
            }
            
        }
        
        kChunk:: ~kChunk() throw()
        {
        }
        
        void * kChunk:: acquire(const size_t block_size) throw()
        {
            assert(block_size<=(blockIncrement<<1));
            assert(stillAvailable>0);
            //std::cerr << "\t\tfirstAvailable0=" << firstAvailable << std::endl;
            uint16_t     *p = &data[firstAvailable*blockIncrement];
            firstAvailable  = *p;
            //std::cerr << "\t\tfirstAvailable1=" << firstAvailable << std::endl;

            --stillAvailable;
            
            memset( p, 0, block_size );
            return p;
        }
        
        void kChunk:: release( void *addr ) throw()
        {
            assert( addr != NULL  );

            uint16_t *to_release = (uint16_t *)addr;
            
            assert( static_cast<ptrdiff_t>(to_release - data) % blockIncrement == 0 );
            
            /** update linking **/
            *to_release    = firstAvailable;
            firstAvailable = static_cast<uint16_t>( (to_release-data) / blockIncrement );
            
            assert( firstAvailable == (to_release-data) / blockIncrement );
            
            /** update status **/
            ++stillAvailable;
        }

        
        
    }
}
