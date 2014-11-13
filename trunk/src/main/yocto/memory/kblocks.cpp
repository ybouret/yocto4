#include "yocto/memory/kblocks.hpp"
#include "yocto/memory/karena.hpp"
#include "yocto/memory/global.hpp"

#include "yocto/code/utils.hpp"
#include "yocto/code/bswap.hpp"

#include <cstring>

#include <iostream>

namespace yocto
{
    namespace memory
    {
        kBlocks:: ~kBlocks() throw()
        {
            //std::cerr << "Cleaning " << num_arenas << " arenas" << std::endl;
            while(num_arenas)
            {
                kArena &A = arenas[--num_arenas];
                A.~kArena();
            }
            kind<global>::release_as(arenas,max_arenas);
            acquiring = releasing = 0;
        }
        
        kBlocks:: kBlocks(const size_t a_chunk_size ) throw() :
        chunk_size(a_chunk_size),
        acquiring(0),
        releasing(0),
        arenas(0),
        num_arenas(0),
        max_arenas(0)
        {
            
        }
        
        
        kArena * kBlocks:: search(const size_t block_size)  throw()
        {
            if(num_arenas)
            {
                kArena *lo = arenas;
                kArena *up = arenas+num_arenas;
                --up;
                while(up>=lo)
                {
                    kArena *mid = lo + static_cast<ptrdiff_t>(up-lo)/2;
                    if(mid->block_size<block_size)
                    {
                        lo = mid+1;
                    }
                    else
                    {
                        if(mid->block_size>block_size)
                        {
                            up = mid-1;
                        }
                        else
                        {
                            assert(block_size==mid->block_size);
                            return mid;
                        }
                    }
                }
                
            }
            
            return NULL;
            
        }
        
        
        void * kBlocks:: acquire(const size_t block_size)
        {
            assert(block_size>0);
            
            if(acquiring && acquiring->block_size == block_size)
            {
                return acquiring->acquire();
            }
            else
            {
                acquiring = search(block_size);
                if(!acquiring)
                {
                    //__________________________________________________________
                    //
                    // need a new arena
                    //__________________________________________________________
                    if(num_arenas>=max_arenas)
                    {
                        size_t  new_max_arenas = max_of<size_t>(MinArenas,max_arenas*2);
                        kArena *new_arenas     = kind<global>::acquire_as<kArena>(new_max_arenas);
                        memcpy(new_arenas,arenas,num_arenas*sizeof(kArena));
                        
                        kind<global>::release_as(arenas, max_arenas);
                        
                        arenas     = new_arenas;
                        max_arenas = new_max_arenas;
                    }
                    assert(num_arenas<max_arenas);
                    acquiring = new (arenas+num_arenas) kArena(block_size,chunk_size);
                    ++num_arenas;
                    
                    //__________________________________________________________
                    //
                    // move the arena into place
                    //__________________________________________________________
                    while(acquiring>arenas)
                    {
                        kArena *prev = acquiring-1;
                        assert(prev->block_size!=acquiring->block_size);
                        if(prev->block_size<acquiring->block_size)
                        {
                            break;
                        }
                        bswap<kArena>(*prev,*acquiring);
                        --acquiring;
                    }
                    assert(block_size==acquiring->block_size);
                    releasing = acquiring; // TODO: improve
                }
                assert(acquiring);
                assert(acquiring->block_size==block_size);
                return acquiring->acquire();
            }
        }

        
        void  kBlocks:: release(void *addr, const size_t block_size) throw()
        {
            assert(addr);
            assert(block_size>0);
            if(releasing&&releasing->block_size==block_size)
            {
                releasing->release(addr);
            }
            else
            {
                releasing = search(block_size);
                assert(releasing);
                assert(releasing->block_size==block_size);
                releasing->release(addr);
            }
        }

        
    }
}
