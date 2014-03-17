#include "yocto/utest/run.hpp"
#include "yocto/mpa/natural.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/sequence/vector.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(mem)
{
    vector<mpn> u;
    for(size_t i=1000;i>0;--i)
    {
        const mpn x( alea_leq(512), as_capacity);
        u.push_back(x);
    }
}
YOCTO_UNIT_TEST_DONE()

