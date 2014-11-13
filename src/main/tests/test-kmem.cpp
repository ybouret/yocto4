#include "yocto/memory/kchunk.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;
using namespace memory;

namespace
{
    struct block_t
    {
        void  *addr;
        size_t size;
    };
    
    template <typename T>
    static inline
    void test_tChunk(const size_t block_size, size_t chunk_size)
    {
        std::cerr << "block_size=" << block_size << ", sizeof(tChunk)=" << sizeof(tChunk<T>) << std::endl;
        void *data = kind<global>::acquire(chunk_size);
        std::cerr << "\tchunk_size  = " << chunk_size << " bytes" << std::endl;
        tChunk<T> ch(data,block_size,chunk_size);
        size_t num_blocks = ch.stillAvailable;
        std::cerr << "\tnum_blocks  = " << num_blocks << std::endl;
        block_t *blk = kind<global>::acquire_as<block_t>(num_blocks);
        
        size_t nb = 0;
        for(size_t iter=0;iter<100;++iter)
        {
            while(ch.stillAvailable)
            {
                blk[nb].size = block_size;
                blk[nb].addr = ch.acquire();
                ++nb;
            }
            
            c_shuffle(blk,nb);
            
            {
                const size_t nk = nb/2;
                for(size_t i=0;i<nk;++i)
                {
                    --nb;
                    ch.release(blk[nb].addr);
                    blk[nb].addr = 0;
                    blk[nb].size = 0;
                }
            }
            
            while(ch.stillAvailable)
            {
                blk[nb].size = block_size;
                blk[nb].addr = ch.acquire();
                ++nb;
            }
            
            c_shuffle(blk,nb);
            
            {
                const size_t nk = nb;
                for(size_t i=0;i<nk;++i)
                {
                    --nb;
                    ch.release(blk[nb].addr);
                }
            }
            assert(0==nb);
        }
        
        
        
        kind<global>::release_as(blk, num_blocks);
        kind<global>::release(data,   chunk_size);
        
    }
    
}



YOCTO_UNIT_TEST_IMPL(kChunk)
{
    size_t chunk_size = 1024;
    
    if(argc>1)
    {
        chunk_size = atol(argv[1]);
    }
    
    for(size_t block_size=1;block_size<=128;++block_size)
    {
        test_tChunk<uint8_t> (block_size, chunk_size);
        test_tChunk<uint16_t>(block_size, chunk_size);
        test_tChunk<size_t>  (block_size, chunk_size);
    }
    
    
}
YOCTO_UNIT_TEST_DONE()



#include "yocto/memory/karena.hpp"

namespace
{
    static inline
    void test_kArena(const size_t block_size, size_t chunk_size)
    {
        kArena A(block_size,chunk_size);
        size_t num_blocks = 40000;
        block_t *blk = kind<global>::acquire_as<block_t>(num_blocks);
        
        size_t nb = 0;
        for(size_t i=0;i<num_blocks;++i)
        {
            blk[nb].size = block_size;
            blk[nb].addr = A.acquire();
            ++nb;
        }
        std::cerr << "block_size=" << block_size << ", " << nb << std::endl;
        
        while(nb>0)
        {
            --nb;
            A.release(blk[nb].addr);
        }
        
        kind<global>::release_as(blk, num_blocks);
    }
    
}

YOCTO_UNIT_TEST_IMPL(kArena)
{
    size_t chunk_size = 1024;
    
    if(argc>1)
    {
        chunk_size = atol(argv[1]);
    }
    
    
    for(size_t block_size=1;block_size<=128;++block_size)
    {
        test_kArena(block_size, chunk_size);
    }
    
}
YOCTO_UNIT_TEST_DONE()



