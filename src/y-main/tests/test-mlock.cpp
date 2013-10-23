#include "yocto/utest/run.hpp"
#include "yocto/memory/locked.hpp"
#include <cstdlib>
#include "yocto/memory/page.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(mlock)
{
	
    memory::locked &vmem = memory::locked::instance();
    std::cerr << "using " << vmem.name << std::endl;
    std::cerr << "page size=" << vmem.page_size << std::endl;
    size_t n = 1;
    void  *addr = vmem.acquire(n);
    std::cerr << "locked @" << addr << " +" << n << std::endl;
    int         *p = (int *)addr;
	const size_t m = n/sizeof(*p);
	for(size_t i=0;i<m;++i)
	{
		p[i] = rand();
	}
	vmem.release(addr, n);
    
    std::cerr << "sizeof(memory::page)=" << sizeof(memory::page) << std::endl;
    std::cerr << "sizeof(memory::page::block_t)=" << sizeof(memory::page::block_t) << std::endl;
    
    memory::page *P = new memory::page(0);
    std::cerr << "safe=" << P->safe << std::endl;
    delete P;
    
    P = new memory::page( vmem.page_size * 4 );
    std::cerr << "safe=" << P->safe << std::endl;
    delete P;
    
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/code/rand.hpp"
#include "yocto/sequence/vector.hpp"

namespace
{
    class pblock
    {
    public:
        void  *addr;
        size_t size;
        inline  pblock() : addr(0), size(0) {}
        inline ~pblock() throw() {}
        inline  pblock( const pblock &other ) throw() : addr(other.addr), size(other.size) {}
    private:
        pblock &operator=(const pblock &);
    };
    
    static inline void
    fill( sequence<pblock> &blocks, memory::page &P )
    {
        for(;;)
        {
            pblock B;
            B.size = alea_lt(3*sizeof(memory::page::block_t));
            std::cerr << "B.size=" << B.size;
            B.addr = P.acquire(B.size);
            if(!B.addr)
                break;
            std::cerr << " => B.size=" << B.size << std::endl;
            blocks.push_back(B);
        }
        
    }
    
}

YOCTO_UNIT_TEST_IMPL(pages)
{
    memory::page *P = new memory::page(0);
    if(!P->is_empty()) throw exception("Page is Not Initially empty");
    
    vector<pblock> blocks;
    for(size_t iter=1;iter<=8;++iter)
    {
        fill(blocks,*P);
        size_t nb = blocks.size();
        std::cerr << "\t\t#BLOCKS=" << nb << std::endl;
        if(nb) c_shuffle( &blocks[1], nb);
        while(blocks.size()>nb/2)
        {
            P->release(blocks.back().addr);
            blocks.pop_back();
        }
        fill(blocks,*P);
        nb = blocks.size();
        std::cerr << "\t\t#BLOCKS=" << nb << std::endl;
        if(nb) c_shuffle( &blocks[1], nb);
        while(blocks.size()>0)
        {
            P->release(blocks.back().addr);
            blocks.pop_back();
        }
        if( !P->is_empty())
        {
            throw exception("Page Should Be Empty");
        }
    }
    delete P;
}
YOCTO_UNIT_TEST_DONE()

