#include "yocto/utest/run.hpp"
#include "yocto/mpa/word2mpn.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(w2m)
{
    std::cerr << std::hex;
    for(int i=0;i<10;++i)
    {
        const uint64_t x = _rand.full<uint64_t>();
        mpa::word2mpn  w(x);
        std::cerr << "x=" << x << std::endl;
        std::cerr << "n=" << w.n << std::endl;
    }
}
YOCTO_UNIT_TEST_DONE()

