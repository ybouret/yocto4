#ifndef YOCTO_MEMORY_KCHUNK_INCLUDED
#define YOCTO_MEMORY_KCHUNK_INCLUDED 1

#include "yocto/code/round.hpp"
#include "yocto/type/ints.hpp"

namespace yocto
{
    namespace memory
    {
        
        
        template <typename T>
        class tChunk
        {
        public:
            typedef T            word_type;
            static  const size_t MaxBlocks = limit_of<word_type>::maximum;
            
            word_type          *data;            //!< first byte of data
            word_type           firstAvailable;  //!< bookeeping
            word_type           stillAvailable;  //!< bookeeping
            const word_type     blockIncrement;  //!< for memory access
            const word_type     providedNumber;  //!< initial count
            
            inline tChunk(void        *data_entry,
                          const size_t block_size,
                          const size_t chunk_size ) throw() :
            data( (word_type *)data_entry ),
            firstAvailable(0),
            stillAvailable(0),
            blockIncrement(0),
            providedNumber(0)
            {
                //______________________________________________________________
                //
                // compute all parameters
                //______________________________________________________________
                const size_t block_round    = YOCTO_ALIGN_FOR_ITEM(word_type,block_size);
                (word_type &)blockIncrement = block_round/sizeof(word_type);
                
                const size_t top_blocks     = chunk_size/block_round;
                stillAvailable              = (MaxBlocks < top_blocks) ? MaxBlocks : top_blocks;
                (word_type &)providedNumber = stillAvailable;
                
                //______________________________________________________________
                //
                // format the chunk
                //______________________________________________________________
                word_type  *q = data;
                for(size_t  i = 0; i != stillAvailable; q += blockIncrement )
                {
                    *q = ++i;
                }
                
            }
            
            inline ~tChunk() throw() {}
            
            //! acquired a zeroed piece of memory for initial block_size
            inline void *acquire() throw()
            {
                assert(stillAvailable>0);
                assert(stillAvailable<=providedNumber);
                word_type     *p = &data[firstAvailable*blockIncrement];
                firstAvailable   = *p;
                
                --stillAvailable;
                
                word_type *q = p;
                for(register size_t i=blockIncrement;i>0;--i)
                {
                    *(q++) = 0;
                }
                return p;
            }
            
            //! release a previously allocated piece of memory
            inline void release(void *addr) throw()
            {
                assert(NULL!=addr);
                assert(stillAvailable<providedNumber);
                word_type *to_release = (word_type *)addr;
                assert( static_cast<ptrdiff_t>(to_release - data) % blockIncrement == 0 );
                
                // restore linking
                *to_release    = firstAvailable;
                firstAvailable = static_cast<word_type>( (to_release-data) / blockIncrement );
                
                assert( firstAvailable == (to_release-data) / blockIncrement );
                
                // update status
                ++stillAvailable;
            }
            
            inline bool owns(const void *addr) const throw()
            {
                const word_type *p = (const word_type *)addr;
                return p >= data && p < data+providedNumber*blockIncrement;
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(tChunk);
        };
        
        typedef tChunk<uint16_t> kChunk;
        
        
    }
}

#endif
