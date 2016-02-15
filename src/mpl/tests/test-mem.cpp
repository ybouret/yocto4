#include "yocto/mpl/types.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/math/complex.hpp"

using namespace yocto;
using namespace mpl;

namespace
{
    struct block_t
    {
        size_t   size;
        uint8_t *addr;
    };
}

YOCTO_UNIT_TEST_IMPL(mem)
{
    size_t   nb  = 1000;
    block_t *blk = memory::kind<memory::global>::acquire_as<block_t>(nb);

    manager &mgr = manager::instance();

    for(size_t iter=0;iter<1000;++iter)
    {
        for(size_t i=0;i<nb;++i)
        {
            blk[i].size = alea_lt(2*manager::max_bytes);
            blk[i].addr = mgr.acquire(blk[i].size);
        }
        c_shuffle(blk,nb);
        for(size_t i=0;i<nb;++i)
        {
            mgr.release(blk[i].addr,blk[i].size);
        }
    }

    memory::kind<memory::global>::release_as(blk,nb);

    array_of< math::complex<double> > cplx(100);
    

}
YOCTO_UNIT_TEST_DONE()
