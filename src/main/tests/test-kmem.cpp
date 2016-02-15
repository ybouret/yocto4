#include "yocto/memory/kchunk.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/sys/wtime.hpp"
#include <cstdlib>

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
    void test_tChunk(const size_t block_size, size_t chunk_size, const char *name)
    {
        std::cerr << "block_size=" << block_size << ", sizeof(tChunk<" << name << ">)=" << sizeof(tChunk<T>) << std::endl;
        void *data = kind<global>::acquire(chunk_size);
        std::cerr << "\tchunk_size     = " << chunk_size << " bytes" << std::endl;
        tChunk<T> ch(data,block_size,chunk_size);
        size_t num_blocks = ch.stillAvailable;
        std::cerr << "\tnum_blocks     = " << num_blocks << std::endl;
        std::cerr << "\tword_size      = " << sizeof(typename tChunk<T>::word_type) << std::endl;
        std::cerr << "\tblockIncrement = " << int(ch.blockIncrement) << std::endl;
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

#if 0
    template <bool ZEROED>
    static inline void kChunkPerf()
    {
        rt_clock chrono;
        chrono.calibrate();
        if(ZEROED)
        {
            std::cerr << "-- ZEROED blocks..." << std::endl;
        }
        else
        {
            std::cerr << "-- DIRTY blocks..." << std::endl;
        }
        size_t chunk_size = 4096;
        for(size_t block_size=1;block_size<=512;block_size*=2)
        {
            std::cerr << "block_size=" << block_size; std::cerr.flush();
            void *data = kind<global>::acquire(chunk_size);


            tChunk<uint16_t,ZEROED> ch(data,block_size,chunk_size);
            size_t   num_blocks = ch.stillAvailable;
            block_t *blk        = kind<global>::acquire_as<block_t>(num_blocks);

            size_t         cycles= 0;
            uint64_t       t64   = 0;
            do
            {
                ++cycles;
                const uint64_t mark = rt_clock::ticks();
                size_t nb = 0;
                while(ch.stillAvailable)
                {
                    //blk[nb].size = block_size;
                    blk[nb].addr = ch.acquire();
                    ++nb;
                }
                t64 += (rt_clock::ticks()-mark);
                c_shuffle(blk,nb);
                while(nb--)
                {
                    ch.release(blk[nb].addr);
                }

                //std::cerr << "t64=" << t64 << std::endl;
                //std::cerr << "tmx=" << chrono(t64) << std::endl;
            }
            while( chrono(t64) < 1.0 );

            kind<global>::release_as<block_t>(blk,num_blocks);
            kind<global>::release(data,chunk_size);
            const double tmx = chrono(t64);
            const double speed = 1e-6 * double(cycles)/tmx;
            std::cerr << "\t\tspeed=" << speed << std::endl;
        }
    }
#endif
    
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
        test_tChunk<uint8_t> (block_size, chunk_size, "uint8_t");
        test_tChunk<uint16_t>(block_size, chunk_size, "uint16_t");
        test_tChunk<size_t>  (block_size, chunk_size, "size_t");
    }

#if 0
    kChunkPerf<true>();
    kChunkPerf<false>();
#endif
    
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
        std::cerr << "block_size=" << block_size << ", " << nb << ", chunk_size=" << chunk_size << std::endl;
        
        for(size_t iter=0;iter<8;++iter)
        {
            c_shuffle(blk,nb);
            for(size_t i=nb/2;i>0;--i)
            {
                --nb;
                A.release(blk[nb].addr);
            }
            
            while(nb<num_blocks)
            {
                blk[nb].size = block_size;
                blk[nb].addr = A.acquire();
                ++nb;
            }
        }
        
        c_shuffle(blk,nb);
        
        
        while(nb>0)
        {
            --nb;
            A.release(blk[nb].addr);
        }
        std::cerr << "Final #num_chunks=" << A.get_num_chunks() << " / " << A.get_max_chunks() << std::endl;
        
        kind<global>::release_as(blk,num_blocks);
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
    std::cerr << "sizeof(kArena)=" << sizeof(kArena) << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/memory/kblocks.hpp"

YOCTO_UNIT_TEST_IMPL(kBlocks)
{
    
    
    size_t num_blocks = 20000;
    size_t chunk_size = 4096;
    
    if(argc>1)
    {
        num_blocks = atol(argv[1]);
    }
    
    if(argc>2)
    {
        chunk_size = atol(argv[2]);
    }
    
   
    
    kBlocks B(chunk_size);
    
    
    block_t *blk = kind<global>::acquire_as<block_t>(num_blocks);
    
#define __PRINT_INFO \
std::cerr << "ChunkSize=" << chunk_size << ", playing with " << num_blocks << " blocks" << std::endl
    __PRINT_INFO;
    size_t nb=0;
    {
        while(nb<num_blocks)
        {
            blk[nb].size = 1+alea_lt(100);
            blk[nb].addr = B.acquire(blk[nb].size);
            ++nb;
        }
        
        for(size_t iter=0;iter<8;++iter)
        {
            c_shuffle(blk,nb);
            for(size_t i=nb/2;i>0;--i)
            {
                --nb;
                B.release(blk[nb].addr,blk[nb].size);
            }
            
            while(nb<num_blocks)
            {
                blk[nb].size = 1+alea_lt(100);
                blk[nb].addr = B.acquire(blk[nb].size);
                ++nb;
            }
        }
        
        c_shuffle(blk,nb);
        while(nb>0)
        {
            --nb;
            B.release(blk[nb].addr,blk[nb].size);
        }
    }
    
    kind<global>::release_as(blk, num_blocks);
    
    
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/object.hpp"

YOCTO_UNIT_TEST_IMPL(kObject)
{
    size_t num_blocks = 20000;
    if(argc>1)
    {
        num_blocks = atol(argv[1]);
    }
    
    block_t *blk = kind<global>::acquire_as<block_t>(num_blocks);
    size_t nb=0;
    {
        while(nb<num_blocks)
        {
            blk[nb].size = 1+alea_lt(2*object::limit_size);
            blk[nb].addr = object::operator new(blk[nb].size);
            ++nb;
        }
        
        for(size_t iter=0;iter<8;++iter)
        {
            c_shuffle(blk,nb);
            for(size_t i=nb/2;i>0;--i)
            {
                --nb;
                object::operator delete(blk[nb].addr,blk[nb].size);
            }
            
            while(nb<num_blocks)
            {
                blk[nb].size =  1+alea_lt(2*object::limit_size);
                blk[nb].addr = object::operator new(blk[nb].size);
                ++nb;
            }
        }
        
        c_shuffle(blk,nb);
        while(nb>0)
        {
            --nb;
            object::operator delete(blk[nb].addr,blk[nb].size);
        }
    }
    
    kind<global>::release_as(blk, num_blocks);
    
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/core/list.hpp"
#include "yocto/sys/wtime.hpp"

namespace
{
#if defined(_MSC_VER)
#pragma warning ( disable : 4351 )
#endif
    template <size_t N>
    class raw_node
    {
    public:
        raw_node *next;
        raw_node *prev;
        char      data[N];
        inline raw_node() throw() : next(0), prev(0), data() {}
        inline ~raw_node() throw() {}
        YOCTO_DISABLE_COPY_AND_ASSIGN(raw_node);
    };
    
    template <size_t N>
    class obj_node
    {
    public:
        YOCTO_MAKE_OBJECT
        obj_node *next;
        obj_node *prev;
        char      data[N];
        inline  obj_node() throw() : next(0), prev(0), data() {}
        inline ~obj_node() throw() {}
        YOCTO_DISABLE_COPY_AND_ASSIGN(obj_node);
    };
    
    template <typename NODE>
    static inline
    double test_node_perf(const size_t num)
    {
        std::cerr << "sizeof node=" << sizeof(NODE) << std::endl;
        wtime chrono;
        chrono.start();
        core::list_of_cpp<NODE> L;
        const double tmx = chrono.query();
        for(size_t i=num;i>0;--i)
        {
            if( alea<double>()>0.5 )
            {
                L.push_back( new NODE() );
            }
            else
            {
                L.push_front( new NODE() );
            }
        }
        L.clear();
        return double(num)/(chrono.query()-tmx);
    }
 
    template <size_t N>
    static inline
    double speed_up(const size_t num)
    {
        const double rt = test_node_perf< raw_node<N> >(num);
        const double ot = test_node_perf< obj_node<N> >(num);
        const double sp = ot/rt;
        std::cerr << "raw_speed=" << rt << std::endl;
        std::cerr << "obj_speed=" << ot << std::endl;
        std::cerr << "\tspeed_up" << sizeof(raw_node<N>) << " = " << sp << std::endl;
        return sp;
    }
    
}

YOCTO_UNIT_TEST_IMPL(kObjPerf)
{
    size_t num_blocks = 10000;
    if(argc>1)
    {
        num_blocks = atol(argv[1]);
    }
    
    speed_up<1>(num_blocks);
    speed_up<10>(num_blocks);
    speed_up<40>(num_blocks);

    
    
}
YOCTO_UNIT_TEST_DONE()


