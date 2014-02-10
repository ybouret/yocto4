#include "yocto/utest/run.hpp"
#include "yocto/threading/simd.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;
using namespace threading;

static inline
void DoSomething( context &ctx )
{
    YOCTO_LOCK(ctx.access);
    std::cerr << "working from " << ctx.size << "." << ctx.rank << std::endl;
}

YOCTO_UNIT_TEST_IMPL(simd)
{
    {
        SIMD simd0;
    }
    
    SIMD simd;
    
    SIMD::Kernel K = cfunctor( DoSomething );
    for(size_t cycle=1+alea_leq(3);cycle>0;--cycle)
    {
        simd.cycle(K);
    }

    
    
}
YOCTO_UNIT_TEST_DONE()
