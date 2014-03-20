#include "yocto/utest/run.hpp"
#include "yocto/mpa/natural.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/sequence/list.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(mem)
{
    vector<mpn> u;
    for(size_t i=10000;i>0;--i)
    {
        const mpn x( alea_leq(512), as_capacity);
        u.push_back(x);
    }
    
    list<mpn> v;
    for(size_t i=1;i<=u.size();++i)
    {
        mpn y;
        y = uint64_t(i*i);
        y = u[i];
        v.push_front(y);
    }
}
YOCTO_UNIT_TEST_DONE()
