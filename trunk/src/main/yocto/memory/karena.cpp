#include "yocto/memory/karena.hpp"

#include "yocto/code/round.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/memory/global.hpp"

#include "yocto/code/bswap.hpp"
#include <cstring>
#include <iostream>

namespace yocto
{
    namespace memory
    {

        //======================================================================
        //
        // ctor
        //
        //======================================================================
        
        kArena:: kArena( const size_t a_block_size, const size_t a_chunk_size ) throw() :
        block_size( a_block_size ),
        chunk_size( max_of(max_of(block_size,sizeof(kChunk)),a_chunk_size)  ),
        available(0),
        acquiring(0),
        releasing(0),
        max_chunks( 0 ),
        num_chunks( 0 ),
        chunks( 0 ),
        add_chunks( chunk_size/sizeof(kChunk) )
        {
            assert(chunk_size>=block_size);
            assert(add_chunks>0);
        }
        
        
        //======================================================================
        //
        // dtor
        //
        //======================================================================
        kArena:: ~kArena() throw()
        {
            while(num_chunks>0)
            {
                kChunk &chk = chunks[--num_chunks];
                memory::global::__free(chk.data);
            }
            kind<global>::release_as(chunks, max_chunks);
        }
        
        //======================================================================
        //
        // get memory
        //
        //======================================================================
        void * kArena:: acquire()
        {
            
            if(available<=0)
            {
                //______________________________________________________________
                //
                //
                // no free blocks
                //
                //______________________________________________________________
               
                //--------------------------------------------------------------
                // check memory
                //--------------------------------------------------------------
                if(num_chunks>=max_chunks)
                {
                    //__________________________________________________________
                    //
                    // need to grow
                    //__________________________________________________________
                    size_t  new_max_chunks = max_chunks + add_chunks;
                    kChunk *new_chunks     = kind<global>::acquire_as<kChunk>(new_max_chunks);
                    
                    memcpy(new_chunks,chunks,num_chunks*sizeof(kChunk));
                    
                    //__________________________________________________________
                    //
                    // preserve caching
                    //__________________________________________________________
                    if(releasing)
                    {
                        releasing = new_chunks + static_cast<ptrdiff_t>(releasing-chunks);
                    }
                    if(acquiring)
                    {
                        acquiring = new_chunks + static_cast<ptrdiff_t>(acquiring-chunks);
                    }
                    kind<global>::release_as<kChunk>(chunks,max_chunks);
                    
                    max_chunks = new_max_chunks;
                    chunks     = new_chunks;
                    //std::cerr << "max_chunks=" << max_chunks << std::endl;
                }
                assert(num_chunks<max_chunks);
                //--------------------------------------------------------------
                // allocate a chunk of memory
                //--------------------------------------------------------------
                void   *buffer = memory::global::__calloc(1, chunk_size);
                acquiring = new (chunks+num_chunks) kChunk(buffer,block_size,chunk_size);
                const size_t added_blocks = acquiring->stillAvailable;
                assert(added_blocks>0);
                ++num_chunks;
                available += added_blocks;
                
                //--------------------------------------------------------------
                // ordering by increasing memory address
                //--------------------------------------------------------------
                while(acquiring>chunks)
                {
                    kChunk *prevChunk = acquiring-1; assert(prevChunk>=chunks);
                    if(prevChunk->data<acquiring->data)
                        break;
                    bswap(*prevChunk,*acquiring);
                    --acquiring;
                }
                assert(added_blocks == acquiring->stillAvailable);
                
                //--------------------------------------------------------------
                // direct acquire
                //--------------------------------------------------------------
                --available;
                return acquiring->acquire();
            }
            else
            {
                assert(acquiring); // never NULL after first acquire
                
                //______________________________________________________________
                //
                //
                // a block is available somewhere
                //
                //______________________________________________________________
                if(acquiring->stillAvailable>0)
                {
                    //__________________________________________________________
                    //
                    // cached !
                    //__________________________________________________________
                    --available;
                    return acquiring->acquire();
                }
                else
                {
                    //__________________________________________________________
                    //
                    // must search
                    //__________________________________________________________
                    const kChunk *bot = chunks;
                    const kChunk *top = chunks+num_chunks;
                    
                    kChunk *scanUp = acquiring+1;
                    kChunk *scanLo = acquiring-1;
                    
                SCAN_BOTH:
                    if(scanLo<bot)
                    {
                        goto SCAN_UP;
                    }
                    
                    if(scanLo->stillAvailable>0)
                    {
                        acquiring = scanLo;
                        goto SCAN_DONE;
                    }
                    --scanLo;
                    
                    if(scanUp>=top)
                    {
                        goto SCAN_LO;
                    }
                    
                    if(scanUp->stillAvailable>0)
                    {
                        acquiring = scanUp;
                        goto SCAN_DONE;
                    }
                    ++scanUp;
                    goto SCAN_BOTH;
                    
                SCAN_UP:
                    assert(scanUp<top);
                    if(scanUp->stillAvailable>0)
                    {
                        acquiring = scanUp;
                        goto SCAN_DONE;
                    }
                    ++scanUp;
                    goto SCAN_UP;
                    
                SCAN_LO:
                    assert(scanLo>=bot);
                    if(scanLo->stillAvailable>0)
                    {
                        acquiring = scanLo;
                        goto SCAN_DONE;
                    }
                    --scanLo;
                    goto SCAN_LO;
                    
                SCAN_DONE:
                    assert(acquiring&&acquiring->stillAvailable>0);
                    --available;
                    return acquiring->acquire();
                }
            }
            
            
        }
        
        
        //======================================================================
        //
        // release memory
        //
        //======================================================================
        kArena::ownership kArena::is_owner(const kChunk *ch, const void *addr) const throw()
        {
            const kChunk::word_type *q    = (const kChunk::word_type *)addr;
            const kChunk::word_type *base = ch->data;
            if(q<base)
            {
                return prevChunk;
            }
            else
            {
                const uint8_t  *top  = static_cast<const uint8_t *>(addr)+chunk_size;
                const uint16_t *last = (const uint16_t *)top;
                if(q>=last)
                {
                    return nextChunk;
                }
                else
                {
                    return selfChunk;
                }
            }
        }
        
        
        void kArena:: release(void *addr) throw()
        {
            assert(addr);
            assert(acquiring); // previously acquired
            
            if(!releasing)
            {
                releasing = acquiring;
            }
            
            //__________________________________________________________________
            //
            // initialize search
            //__________________________________________________________________
            kChunk *lo = chunks;            assert(releasing>=lo);
            kChunk *up = chunks+num_chunks; assert(releasing<up);

            
            switch (is_owner(releasing,addr))
            {
                case prevChunk:
                    //std::cerr << "Releasing@Prev" << std::endl;
                    up = releasing;
                    break;
                    
                case selfChunk:
                    // cached !
                    //std::cerr << "Releasing: Cached" << std::endl;
                    goto FOUND;
                    
                case nextChunk:
                    //std::cerr << "Releasing@Next" << std::endl;
                    lo = releasing+1;
                    break;
            }
            
            //__________________________________________________________________
            //
            // finalize search
            //__________________________________________________________________
            --up;
            assert(lo<=up);
            while(true)
            {
                kChunk *mid = lo + (static_cast<ptrdiff_t>(up-lo))/2;
                switch(is_owner(mid, addr) )
                {
                    case prevChunk:
                        up = mid-1;
                        break;
                        
                    case selfChunk:
                        releasing = mid;
                        goto FOUND;
                        
                    case nextChunk:
                        lo = mid+1;
                        break;
                }
            }
            
        FOUND:
            assert(releasing);
            assert(selfChunk==is_owner(releasing,addr));
            releasing->release(addr);
            ++available;
        }

        
    }
}
