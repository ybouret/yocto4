#include "yocto/utest/run.hpp"
#include "yocto/memory/locked.hpp"
#include <cstdlib>
#include "yocto/core/list.hpp"

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
}
YOCTO_UNIT_TEST_DONE()
